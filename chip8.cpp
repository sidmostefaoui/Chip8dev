#include "chip8.h"
#include <ctime>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <iostream>
#include "asm.h"

namespace emu
{

	Chip8::Chip8(const std::string& rom)
		: memory(), V(), I(), pc(0x200), sp(0x4E), st(60), dt(60),
			keyboard(), framebuffer_(), opcode_()
	{
		constexpr std::array<uint8_t, 80> fontset = {
			0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
			0x20, 0x60, 0x20, 0x20, 0x70, // 1
			0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
			0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
			0x90, 0x90, 0xF0, 0x10, 0x10, // 4
			0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
			0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
			0xF0, 0x10, 0x20, 0x40, 0x40, // 7
			0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
			0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
			0xF0, 0x90, 0xF0, 0x90, 0x90, // A
			0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
			0xF0, 0x80, 0x80, 0x80, 0xF0, // C
			0xE0, 0x90, 0x90, 0x90, 0xE0, // D
			0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
			0xF0, 0x80, 0xF0, 0x80, 0x80  // F
		};

		// load fontset into memory
		std::copy(std::begin(fontset), std::end(fontset), std::begin(memory));
		
		auto f = std::basic_ifstream<uint8_t>(rom, std::ios::binary);
		if (!f) throw std::runtime_error("Cannot Load Program");
		for (int i = 0x200; f.read(memory.data() + i, 1); i++);

		srand(clock()); 
	}

	void Chip8::update_keyboard(const Keyboard& new_keyboard)
	{
		keyboard = new_keyboard;
	}

	void Chip8::emulate_cycle(const float delta_time)
	{
		/* fetch instruction */
		uint8_t hi = memory[pc];
		uint8_t lo = memory[static_cast<size_t>(pc) + 1];
		opcode_ = { hi, lo };
		inst_ = Asm::decode(opcode_);
		execute();

		static float counter = 0;
		counter += delta_time;

		if (counter >= 1.0f / 60)
		{
			if (dt > 0) dt--;
			if (st > 0) st--;
			counter = 0;
		}
	}

	void Chip8::execute()
	{
		using inst_fn_t = std::function<void(void)>;
		using inst_map_t = std::array<inst_fn_t, static_cast<size_t>(Asm::Instruction::SIZE)>;

		inst_map_t instruction_set = {
			[this](){ cls(); },
			[this](){ ret(); },
			[this](){ jp(); },
			[this](){ call_nnn(); },
			[this](){ se_vx_kk(); },
			[this](){ sne_vx_kk(); },
			[this](){ se_vx_vy(); },
			[this](){ ld_vx_kk(); },
			[this](){ add_vx_kk(); },
			[this](){ ld_vx_vy(); },
			[this](){ or_vx_vy(); },
			[this](){ and_vx_vy(); },
			[this](){ xor_vx_vy(); },
			[this](){ add_vx_vy(); },
			[this](){ sub_vx_vy(); },
			[this](){ shr_vx(); },
			[this](){ subn_vx_vy(); },
			[this](){ shl_vx(); },
			[this](){ sne_vx_vy(); },
			[this](){ ld_i_nnn(); },
			[this](){ jp_v0_nnn(); },
			[this](){ rnd_vx_kk(); },
			[this](){ drw_vx_vy(); },
			[this](){ skp_vx(); },
			[this](){ sknp_vx(); },
			[this](){ ld_vx_dt(); },
			[this](){ ld_vx_k(); },
			[this](){ ld_dt_vx(); },
			[this](){ ld_st_vx(); },
			[this](){ add_i_vx(); },
			[this](){ ld_f_vx(); },
			[this](){ ld_b_vx(); },
			[this](){ ld_i_vx(); },
			[this](){ ld_vx_i(); }
		};

		instruction_set[static_cast<size_t>(inst_)]();
	}

	/* clear the display */
	void Chip8::cls()
	{
		for (size_t y = 0; y < 32; y++)
		{
			for (size_t x = 0; x < 64; x++)
			{
				framebuffer_[y * 64 + x] = 0;
			}
		}
		pc += 2;
	}

	/* return from a subroutine */
	void Chip8::ret()
	{
		uint16_t hi = memory[sp];
		uint16_t lo = memory[static_cast<size_t>(sp) + 1];
		pc = (hi << 8) | lo;
		sp -= 2;
	}

	/* jump to location nnn */
	void Chip8::jp()
	{
		pc = opcode_.nnn();
	}

	/* call subroutine at nnn */
	void Chip8::call_nnn()
	{
		sp += 2;
		uint16_t ret_adr = pc + 2;
		memory[sp] = static_cast<uint8_t>(ret_adr >> 8); // hi
		memory[static_cast<size_t>(sp) + 1] = static_cast<uint8_t>(ret_adr); // lo
		pc = opcode_.nnn();
	}

	/* Skip next instruction if Vx = kk */
	void Chip8::se_vx_kk()
	{
		if (V[opcode_.x()] == opcode_.kk())
		{
			pc += 2;
		}
		pc += 2;
	}

	/* Skip next instruction if Vx != kk */
	void Chip8::sne_vx_kk()
	{
		if (V[opcode_.x()] != opcode_.kk())
		{
			pc += 2;
		}
		pc += 2;
	}

	/* Skip next instruction if Vx = Vy */
	void Chip8::se_vx_vy()
	{
		if (V[opcode_.x()] == V[opcode_.y()])
		{
			pc += 2;
		}
		pc += 2;
	}

	/* Set Vx = kk */
	void Chip8::ld_vx_kk()
	{
		V[opcode_.x()] = opcode_.kk();
		pc += 2;
	}

	/* Set Vx = Vx + kk */
	void Chip8::add_vx_kk()
	{
		V[opcode_.x()] += opcode_.kk();
		pc += 2;
	}

	/* Set Vx = Vy */
	void Chip8::ld_vx_vy()
	{
		V[opcode_.x()] = V[opcode_.y()];
		pc += 2;
	}

	/* Set Vx = Vx | Vy  (bitwise or) */
	void Chip8::or_vx_vy()
	{
		V[opcode_.x()] |= V[opcode_.y()];
		pc += 2;
	}

	/* Set Vx = Vx & Vy  (bitwise and) */
	void Chip8::and_vx_vy()
	{
		V[opcode_.x()] &= V[opcode_.y()];
		pc += 2;
	}

	/* Set Vx = Vx ^ Vy  (bitwise xor) */
	void Chip8::xor_vx_vy()
	{
		V[opcode_.x()] ^= V[opcode_.y()];
		pc += 2;
	}

	/* Set Vx = Vx + Vy */
	void Chip8::add_vx_vy()
	{
		uint16_t sum = static_cast<uint16_t>(V[opcode_.x()]) + V[opcode_.y()];
		V[0xF] = static_cast<uint8_t>(sum >> 8); // carry is stored in VF 
		V[opcode_.x()] = static_cast<uint8_t>(sum & 0x00FF);
		pc += 2;
	}

	/* Set Vx = Vx - Vy */
	void Chip8::sub_vx_vy()
	{
		V[0xF] = V[opcode_.x()] > V[opcode_.y()]; // VF = NOT borrow
		V[opcode_.x()] -= V[opcode_.y()];
		pc += 2;
	}

	/* shift right Vx by 1 */
	void Chip8::shr_vx()
	{
		V[0xF] = V[opcode_.x()] & 0x01; // VF = least significant (rightmost) bit of Vx
		V[opcode_.x()] = V[opcode_.x()] >> 1;
		pc += 2;
	}

	/* Set Vx = Vy - Vx */
	void Chip8::subn_vx_vy()
	{
		V[0xF] = V[opcode_.y()] > V[opcode_.x()]; // VF = NOT borrow
		V[opcode_.x()] = V[opcode_.y()] - V[opcode_.x()];
		pc += 2;
	}

	/* shift left Vx by 1 */
	void Chip8::shl_vx()
	{
		/* maybe wrong (compatibility with different interpreters */
		V[0xF] = V[opcode_.x()] & 0x80; // VF = most significant (leftmost) bit of Vx
		V[opcode_.x()] = V[opcode_.x()] << 1;
		pc += 2;
		std::cout << "instruction: 8xyE shl Vx, \n";
	}

	/* skip next instruction if Vx != Vy */
	void Chip8::sne_vx_vy()
	{
		if (V[opcode_.x()] != V[opcode_.y()])
		{
			pc += 2;
		}
		pc += 2;
	}

	/* set I = nnn */
	void Chip8::ld_i_nnn()
	{
		I = opcode_.nnn();
		pc += 2;
	}

	/* jump to location nnn + V0 */
	void Chip8::jp_v0_nnn()
	{
		pc = opcode_.nnn() + V[0];
	}

	/* set Vx = random byte & kk (bitwise AND) */
	void Chip8::rnd_vx_kk()
	{
		V[opcode_.x()] = static_cast<uint8_t>(rand() % 256) & opcode_.kk();
		pc += 2;
	}

	/* draws sprite at offset I in memory on screen at (x, y)
		http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4 */
	void Chip8::drw_vx_vy()
	{
			V[0xF] = 0; // Vf is zero if no pixels are erased
			uint8_t n = opcode_.n(); // sprite size in bytes
			size_t Vx = V[opcode_.x()]; // x starting pos for drawing is value of Vx
			size_t Vy = V[opcode_.y()]; // y starting pos for drawing is value of Vy

			for (uint8_t row = 0; row < n; row++) // number of rows is size of sprite
			{
				/* each byte from sprite goes in respective row */
				size_t offset = static_cast<size_t>(I) + row;
				uint8_t byte = memory[offset];  

				for (uint8_t col = 0; col < 8; col++)
				{
					/* modulo is used so x and y wrap around framebuffer edges */
					size_t x = (Vx + col) % 64;
					size_t y = (Vy + row) % 32;

					/* extract bit for respective col from byte */
					uint8_t bit = (byte & (0x80 >> col)) >> (7 - col); 

					/* new pixel value is bit from framebuffer or (bitwise) bit from sprite */
					uint8_t px_old = framebuffer_[y * 64 + x];
					uint8_t px_new = px_old ^ bit;
					
					// if a pixel is erased Vf is set to 1
					if (px_old == 1 && px_new == 0)
					{
						V[0xF] = 1;
					}

					/* update pixel value in framebuffer */
					framebuffer_[y * 64 + x] = px_new;
				}
			}

			pc += 2;
	}

	/* skip next instruction if keys[Vx] is pressed */
	void Chip8::skp_vx()
	{
		if (keyboard[V[opcode_.x()]])
		{
			pc += 2;
		}
		pc += 2;
	}

	/* skip next instruction if keys[Vx] is not pressed */
	void Chip8::sknp_vx()
	{
		if (!keyboard[V[opcode_.x()]])
		{
			pc += 2;
		}
		pc += 2;
	}

	/* copy dt register into Vx */
	void Chip8::ld_vx_dt()
	{
		V[opcode_.x()] = dt;
		pc += 2;
	}

	/* wait until a key is pressed and copy it's value into Vx */
	void Chip8::ld_vx_k()
	{
		for (uint8_t i = 0; i < keyboard.size(); i++)
		{
			if (keyboard[i] )
			{
				V[opcode_.x()] = i;
				pc += 2;
			}
		}
	}

	/* copy Vx register into dt */
	void Chip8::ld_dt_vx()
	{
		dt = V[opcode_.x()];
		pc += 2;
	}

	/* copy Vx register into st */
	void Chip8::ld_st_vx()
	{
		st = V[opcode_.x()];
		pc += 2;
	}

	/* set I = I + Vx */
	void Chip8::add_i_vx()
	{
		I += V[opcode_.x()];
		pc += 2;
	}

	/* set I = sprite location for number stored in Vx*/
	void Chip8::ld_f_vx()
	{
		/* fontset sprites are stored at 0x0000 and each sprite is 5 bytes */
		I = V[opcode_.x()] * 5;
		pc += 2;
	}

	/* copy bcd representation of Vx into locations I, I+1 and I+2 */
	void Chip8::ld_b_vx()
	{
		uint8_t remainder = V[opcode_.x()];
		uint8_t hundreds = remainder / 100;
		
		remainder %= 100;
		uint8_t tens = remainder / 10;

		remainder %= 10;
		uint8_t ones = remainder;

		memory[I] = hundreds;
		memory[static_cast<size_t>(I) + 1] = tens;
		memory[static_cast<size_t>(I) + 2] = ones;

		pc += 2;
	}

	/* write register V0 ... Vx into memory at location I */
	void Chip8::ld_i_vx()
	{
		for (int i = 0; i <= opcode_.x(); i++)
		{
			memory[static_cast<size_t>(I) + i] = V[i];
		}
		pc += 2;
	}

	/* read register V0 ... Vx from memory at location I */
	void Chip8::ld_vx_i()
	{
		for (int i = 0; i <= opcode_.x(); i++)
		{
			V[i] = memory[static_cast<size_t>(I) + i];
		}
		pc += 2;
	}
}
