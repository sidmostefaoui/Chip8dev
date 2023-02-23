#include "chip8.h"
#include <ctime>
#include <stdexcept>
#include <functional>
#include <fstream>
#include <iostream>

namespace chip8dev
{

	Chip8::Chip8(const std::string& rom)
		: memory(), stack(), V(), I(), pc(0x200), sp(0), st(60), dt(60),
			opcode_(), keyboard(), framebuffer_()
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
		
		// load program into memory
		auto f = std::basic_ifstream<uint8_t>(rom, std::ios::binary);
		if (!f) throw std::runtime_error("Cannot Load Program");
		for (int i = 0x200; f.read(memory.data() + i, 1); i++);

		srand(clock()); 
	}

	void Chip8::update_pressed_keys(const Keyboard& new_keyboard)
	{
		keyboard = new_keyboard;
	}

	const Framebuffer& Chip8::framebuffer() const
	{
		return framebuffer_;
	}

	void Chip8::emulate_cycle()
	{
		opcode_ = fetch();
		execute(decode(opcode_), opcode_);
		if (dt > 0) dt--;
		if (st > 0) st--;
	}

	Opcode Chip8::fetch()
	{
		uint8_t hi = memory[pc];
		uint8_t lo = memory[static_cast<size_t>(pc) + 1];
		return { hi, lo };
	}

	enum class Instruction {
		_00E0 = 0, // cls
		_00EE, // ret
		_1NNN, // jp nnn
		_2NNN, // call nnn
		_3XKK, // se Vx, kk
		_4XKK, // sne Vx, kk
		_5XY0, // se Vx, Vy
		_6XKK, // ld Vx, kk
		_7XKK, // add Vx, kk
		_8XY0, // ld Vx, Vy
		_8XY1, // or Vx, Vy
		_8XY2, // and Vx, Vy
		_8XY3, // xor Vx, Vy
		_8XY4, // add Vx, Vy
		_8XY5, // sub Vx, Vy
		_8XY6, // shr Vx
		_8XY7, // subn Vx, Vy
		_8XYE, // shl Vx
		_9XY0, // sne Vx, Vy
		_ANNN, // ld I, nnn
		_BNNN, // jp V0, nnn
		_CXKK, // rnd Vx, kk
		_DXYN, // drw Vx, Vy, n
		_EX9E, // skp Vx
		_EXA1, // sknp Vx
		_FX07, // ld Vx, dt
		_FX0A, // ld Vx, key
		_FX15, // ld dt, Vx
		_FX18, // ld st, Vx
		_FX1E, // add I, Vx
		_FX29, // ld F, Vx
		_FX33, // ld B, Vx
		_FX55, // ld [I], Vx
		_FX65, // ld Vx, I
		SIZE
	};

	Instruction Chip8::decode(const Opcode opcode)
	{
		const uint8_t hi = opcode.hi;
		const uint8_t lo = opcode.lo;
		const uint8_t hi_left = opcode.hi_left();
		const uint8_t lo_left = opcode.lo_left();
		const uint8_t lo_right = opcode.lo_right();

		if (hi == 0x00 && lo == 0xE0) return Instruction::_00E0;
		if (hi == 0x00 && lo == 0xEE) return Instruction::_00EE;
		if (hi_left == 0x1) return Instruction::_1NNN;
		if (hi_left == 0x2) return Instruction::_2NNN;
		if (hi_left == 0x3) return Instruction::_3XKK;
		if (hi_left == 0x4) return Instruction::_4XKK;
		if (hi_left == 0x5 && lo_right == 0x0) return Instruction::_5XY0;
		if (hi_left == 0x6) return Instruction::_6XKK;
		if (hi_left == 0x7) return Instruction::_7XKK;
		if (hi_left == 0x8 && lo_right == 0x0) return Instruction::_8XY0;
		if (hi_left == 0x8 && lo_right == 0x1) return Instruction::_8XY1;
		if (hi_left == 0x8 && lo_right == 0x2) return Instruction::_8XY2;
		if (hi_left == 0x8 && lo_right == 0x3) return Instruction::_8XY3;
		if (hi_left == 0x8 && lo_right == 0x4) return Instruction::_8XY4;
		if (hi_left == 0x8 && lo_right == 0x5) return Instruction::_8XY5;
		if (hi_left == 0x8 && lo_right == 0x6) return Instruction::_8XY6;
		if (hi_left == 0x8 && lo_right == 0x7) return Instruction::_8XY7;
		if (hi_left == 0x8 && lo_right == 0xE) return Instruction::_8XYE;
		if (hi_left == 0x9 && lo_right == 0x0) return Instruction::_9XY0;
		if (hi_left == 0xA) return Instruction::_ANNN;
		if (hi_left == 0xB) return Instruction::_BNNN;
		if (hi_left == 0xC) return Instruction::_CXKK;
		if (hi_left == 0xD) return Instruction::_DXYN;
		if (hi_left == 0xE && lo_left == 0x9 && lo_right == 0xE) return Instruction::_EX9E;
		if (hi_left == 0xE && lo_left == 0xA && lo_right == 0x1) return Instruction::_EXA1;
		if (hi_left == 0xF && lo_left == 0x0 && lo_right == 0x7) return Instruction::_FX07;
		if (hi_left == 0xF && lo_left == 0x0 && lo_right == 0xA) return Instruction::_FX0A;
		if (hi_left == 0xF && lo_left == 0x1 && lo_right == 0x5) return Instruction::_FX15;
		if (hi_left == 0xF && lo_left == 0x1 && lo_right == 0x8) return Instruction::_FX18;
		if (hi_left == 0xF && lo_left == 0x1 && lo_right == 0xE) return Instruction::_FX1E;
		if (hi_left == 0xF && lo_left == 0x2 && lo_right == 0x9) return Instruction::_FX29;
		if (hi_left == 0xF && lo_left == 0x3 && lo_right == 0x3) return Instruction::_FX33;
		if (hi_left == 0xF && lo_left == 0x5 && lo_right == 0x5) return Instruction::_FX55;
		if (hi_left == 0xF && lo_left == 0x6 && lo_right == 0x5) return Instruction::_FX65;

		throw std::runtime_error("Wrong or unsupported opcode in rom");
	}

	void Chip8::execute(const Instruction inst, const Opcode opcode)
	{
		using inst_fn_t = std::function<void(const Opcode)>;
		using inst_map_t = std::array<inst_fn_t, static_cast<size_t>(Instruction::SIZE)>;

		inst_map_t instruction_set = {
			[this](const Opcode opcode){ cls(opcode); },
			[this](const Opcode opcode){ ret(opcode); },
			[this](const Opcode opcode){ jp(opcode); },
			[this](const Opcode opcode){ call_nnn(opcode); },
			[this](const Opcode opcode){ se_vx_kk(opcode); },
			[this](const Opcode opcode){ sne_vx_kk(opcode); },
			[this](const Opcode opcode){ se_vx_vy(opcode); },
			[this](const Opcode opcode){ ld_vx_kk(opcode); },
			[this](const Opcode opcode){ add_vx_kk(opcode); },
			[this](const Opcode opcode){ ld_vx_vy(opcode); },
			[this](const Opcode opcode){ or_vx_vy(opcode); },
			[this](const Opcode opcode){ and_vx_vy(opcode); },
			[this](const Opcode opcode){ xor_vx_vy(opcode); },
			[this](const Opcode opcode){ add_vx_vy(opcode); },
			[this](const Opcode opcode){ sub_vx_vy(opcode); },
			[this](const Opcode opcode){ shr_vx(opcode); },
			[this](const Opcode opcode){ subn_vx_vy(opcode); },
			[this](const Opcode opcode){ shl_vx(opcode); },
			[this](const Opcode opcode){ sne_vx_vy(opcode); },
			[this](const Opcode opcode){ ld_i_nnn(opcode); },
			[this](const Opcode opcode){ jp_v0_nnn(opcode); },
			[this](const Opcode opcode){ rnd_vx_kk(opcode); },
			[this](const Opcode opcode){ drw_vx_vy(opcode); },
			[this](const Opcode opcode){ skp_vx(opcode); },
			[this](const Opcode opcode){ sknp_vx(opcode); },
			[this](const Opcode opcode){ ld_vx_dt(opcode); },
			[this](const Opcode opcode){ ld_vx_k(opcode); },
			[this](const Opcode opcode){ ld_dt_vx(opcode); },
			[this](const Opcode opcode){ ld_st_vx(opcode); },
			[this](const Opcode opcode){ add_i_vx(opcode); },
			[this](const Opcode opcode){ ld_f_vx(opcode); },
			[this](const Opcode opcode){ ld_b_vx(opcode); },
			[this](const Opcode opcode){ ld_i_vx(opcode); },
			[this](const Opcode opcode){ ld_vx_i(opcode); }
		};

		instruction_set[static_cast<size_t>(inst)](opcode);
	}

	/* clear the display */
	void Chip8::cls(const Opcode)
	{
		for (size_t y = 0; y < 32; y++)
		{
			for (size_t x = 0; x < 64; x++)
			{
				framebuffer_[y][x] = 0;
			}
		}
		pc += 2;
		std::cout << "instruction: 00E0 cls\n";
	}

	/* return from a subroutine */
	void Chip8::ret(const Opcode)
	{
		pc = stack[sp];
		sp -= 1;
		std::cout << "instruction: 00EE ret\n";
	}

	/* jump to location nnn */
	void Chip8::jp(const Opcode opcode)
	{
		pc = opcode.nnn();
		std::cout << "instruction: 1NNN jp nnn\n";
	}

	/* call subroutine at nnn */
	void Chip8::call_nnn(const Opcode opcode)
	{
		sp += 1;
		stack[sp] = pc + 2;
		pc = opcode.nnn();
		std::cout << "instruction: 2NNN call nnn\n";
	}

	/* Skip next instruction if Vx = kk */
	void Chip8::se_vx_kk(const Opcode opcode)
	{
		if (V[opcode.x()] == opcode.kk())
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: 3XNN se Vx, kk\n";
	}

	/* Skip next instruction if Vx != kk */
	void Chip8::sne_vx_kk(const Opcode opcode)
	{
		if (V[opcode.x()] != opcode.kk())
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: 4XNN sne Vx, kk\n";
	}

	/* Skip next instruction if Vx = Vy */
	void Chip8::se_vx_vy(const Opcode opcode)
	{
		if (V[opcode.x()] == V[opcode.y()])
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: 5xy0 se Vx, vy\n";
	}

	/* Set Vx = kk */
	void Chip8::ld_vx_kk(const Opcode opcode)
	{
		V[opcode.x()] = opcode.kk();
		pc += 2;
		std::cout << "instruction: 6xkk ld Vx, kk\n";
	}

	/* Set Vx = Vx + kk */
	void Chip8::add_vx_kk(const Opcode opcode)
	{
		V[opcode.x()] += opcode.kk();
		pc += 2;
		std::cout << "instruction: 7xkk add Vx, kk\n";
	}

	/* Set Vx = Vy */
	void Chip8::ld_vx_vy(const Opcode opcode)
	{
		V[opcode.x()] = V[opcode.y()];
		pc += 2;
		std::cout << "instruction: 8xy0 ld Vx, Vy\n";
	}

	/* Set Vx = Vx | Vy  (bitwise or) */
	void Chip8::or_vx_vy(const Opcode opcode)
	{
		V[opcode.x()] |= V[opcode.y()];
		pc += 2;
		std::cout << "instruction: 8xy1 or Vx, Vy\n";
	}

	/* Set Vx = Vx & Vy  (bitwise and) */
	void Chip8::and_vx_vy(const Opcode opcode)
	{
		V[opcode.x()] &= V[opcode.y()];
		pc += 2;
		std::cout << "instruction: 8xy2 and Vx, Vy\n";
	}

	/* Set Vx = Vx ^ Vy  (bitwise xor) */
	void Chip8::xor_vx_vy(const Opcode opcode)
	{
		V[opcode.x()] ^= V[opcode.y()];
		pc += 2;
		std::cout << "instruction: 8xy3 xor Vx, Vy\n";
	}

	/* Set Vx = Vx + Vy */
	void Chip8::add_vx_vy(const Opcode opcode)
	{
		uint16_t sum = static_cast<uint16_t>(V[opcode.x()]) + V[opcode.y()];
		V[0xF] = static_cast<uint8_t>(sum >> 8); // carry is stored in VF 
		V[opcode.x()] = static_cast<uint8_t>(sum & 0x00FF);
		pc += 2;
		std::cout << "instruction: 8xy4 add Vx, Vy\n";
	}

	/* Set Vx = Vx - Vy */
	void Chip8::sub_vx_vy(const Opcode opcode)
	{
		V[0xF] = V[opcode.x()] > V[opcode.y()]; // VF = NOT borrow
		V[opcode.x()] -= V[opcode.y()];
		pc += 2;
		std::cout << "instruction: 8xy5 sub Vx, vy\n";
	}

	/* shift right Vx by 1 */
	void Chip8::shr_vx(const Opcode opcode)
	{
		V[0xF] = V[opcode.x()] & 0x01; // VF = least significant (rightmost) bit of Vx
		V[opcode.x()] = V[opcode.x()] >> 1;
		pc += 2;
		std::cout << "instruction: 8xy6 shr Vx, \n";
	}

	/* Set Vx = Vy - Vx */
	void Chip8::subn_vx_vy(const Opcode opcode)
	{
		V[0xF] = V[opcode.y()] > V[opcode.x()]; // VF = NOT borrow
		V[opcode.x()] = V[opcode.y()] - V[opcode.x()];
		pc += 2;
		std::cout << "instruction: 8xy7 sub Vx, Vy\n";
	}

	/* shift left Vx by 1 */
	void Chip8::shl_vx(const Opcode opcode)
	{
		/* maybe wrong (compatibility with different interpreters */
		V[0xF] = V[opcode.x()] & 0x80; // VF = most significant (leftmost) bit of Vx
		V[opcode.x()] = V[opcode.x()] << 1;
		pc += 2;
		std::cout << "instruction: 8xyE shl Vx, \n";
	}

	/* skip next instruction if Vx != Vy */
	void Chip8::sne_vx_vy(const Opcode opcode)
	{
		if (V[opcode.x()] != V[opcode.y()])
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: 9xy0 sne Vx, Vy\n";
	}

	/* set I = nnn */
	void Chip8::ld_i_nnn(const Opcode opcode)
	{
		I = opcode.nnn();
		pc += 2;
		std::cout << "instruction: Annn ld I, nnn\n";
	}

	/* jump to location nnn + V0 */
	void Chip8::jp_v0_nnn(const Opcode opcode)
	{
		pc = opcode.nnn() + V[0];
		std::cout << "instruction: Bnnn jp V0, nnn\n";
	}

	/* set Vx = random byte & kk (bitwise AND) */
	void Chip8::rnd_vx_kk(const Opcode opcode)
	{
		V[opcode.x()] = static_cast<uint8_t>(rand() % 256) & opcode.kk();
		pc += 2;
		std::cout << "instruction: Cxkk rnd Vx, kk\n";
	}

	/* helper function for drw_vx_vy:
		extracts all bits in a byte into array of bools */
	std::array<uint8_t, 8> unpack_byte(uint8_t byte)
	{
		std::array<uint8_t, 8> bits = {};
		bits[0] = byte & 0x80;
		bits[1] = byte & 0x40;
		bits[2] = byte & 0x20;
		bits[3] = byte & 0x10;
		bits[4] = byte & 0x08;
		bits[5] = byte & 0x04;
		bits[6] = byte & 0x02;
		bits[7] = byte & 0x01;
		return bits;
	}

	/* draws sprite at offset I in memory on screen at (x, y)
		http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4 */
	void Chip8::drw_vx_vy(const Opcode opcode)
		{
			V[0xF] = 0; // Vf is zero if no pixels are erased
			uint8_t n = opcode.n(); // sprite size in bytes
			uint8_t Vx = V[opcode.x()]; // x starting pos for drawing is value of Vx
			uint8_t Vy = V[opcode.y()]; // y starting pos for drawing is value of Vy

			for (uint8_t row = 0; row < n; row++) // number of rows is size of sprite
			{
				size_t offset = static_cast<size_t>(I) + row;
				uint8_t byte = memory[offset];  // each byte from sprite starts in new row

				std::array<uint8_t, 8> bits = unpack_byte(byte); // each bit is a pixel

				for (uint8_t col = 0; col < 8; col++) // each pixel goes in new column
				{
					size_t x = (static_cast<size_t>(Vx) + col) % 64;
					size_t y = (static_cast<size_t>(Vy) + row) % 32;

					uint8_t& px = framebuffer_[y][x]; // framebuffer is ordered row first so [y][x] is (x, y)
					uint8_t px_old = px;

					px ^= bits[col];  // update new pixel
					
					// if a pixel is erased Vf is set to 1
					if (px_old == 1 && px == 0)
					{
						V[0xF] = 1;
					}
				}
			}

			pc += 2;
			std::cout << "instruction: Dxyn drw Vx, Vy, n\n";
	}

	/* skip next instruction if keys[Vx] is pressed */
	void Chip8::skp_vx(const Opcode opcode)
	{
		if (keyboard[V[opcode.x()]] == true)
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: Ex9E skp Vx\n";
	}

	/* skip next instruction if keys[Vx] is not pressed */
	void Chip8::sknp_vx(const Opcode opcode)
	{
		if (keyboard[V[opcode.x()]] == false)
		{
			pc += 2;
		}
		pc += 2;
		std::cout << "instruction: ExA1 sknp Vx\n";
	}

	/* copy dt register into Vx */
	void Chip8::ld_vx_dt(const Opcode opcode)
	{
		V[opcode.x()] = dt;
		pc += 2;
		std::cout << "instruction: Fx07 ld Vx, dt\n";
	}

	/* wait until a key is pressed and copy it's value into Vx */
	void Chip8::ld_vx_k(const Opcode opcode)
	{
		for (uint8_t i = 0; i < keyboard.size(); i++)
		{
			if (keyboard[i] == true)
			{
				V[opcode.x()] = i;
				pc += 2;
			}
		}
		std::cout << "instruction: Fx0A ld Vx, k\n";
	}

	/* copy Vx register into dt */
	void Chip8::ld_dt_vx(const Opcode opcode)
	{
		dt = V[opcode.x()];
		pc += 2;
		std::cout << "instruction: Fx15 ld dt, Vx\n";
	}

	/* copy Vx register into st */
	void Chip8::ld_st_vx(const Opcode opcode)
	{
		st = V[opcode.x()];
		pc += 2;
		std::cout << "instruction: Fx18 ld st, Vx\n";
	}

	/* set I = I + Vx */
	void Chip8::add_i_vx(const Opcode opcode)
	{
		I += V[opcode.x()];
		pc += 2;
		std::cout << "instruction: Fx1E add I, Vx\n";
	}

	/* set I = sprite location for number stored in Vx*/
	void Chip8::ld_f_vx(const Opcode opcode)
	{
		/* fontset sprites are stored at 0x0000 and each sprite is 5 bytes */
		I = V[opcode.x()] * 5;
		pc += 2;
		std::cout << "instruction: Fx29 ld F, Vx\n";
	}

	/* copy bcd representation of Vx into locations I, I+1 and I+2 */
	void Chip8::ld_b_vx(const Opcode opcode)
	{
		uint8_t remainder = V[opcode.x()];
		uint8_t hundreds = remainder / 100;
		
		remainder %= 100;
		uint8_t tens = remainder / 10;

		remainder %= 10;
		uint8_t ones = remainder;

		memory[I] = hundreds;
		memory[static_cast<size_t>(I) + 1] = tens;
		memory[static_cast<size_t>(I) + 2] = ones;

		pc += 2;
		std::cout << "instruction: Fx33 ld B, Vx\n";
	}

	/* write register V0 ... Vx into memory at location I */
	void Chip8::ld_i_vx(const Opcode opcode)
	{
		for (int i = 0; i <= opcode.x(); i++)
		{
			memory[static_cast<size_t>(I) + i] = V[i];
		}
		pc += 2;
		std::cout << "instruction: Fx55 ld [I], Vx\n";
	}

	/* read register V0 ... Vx from memory at location I */
	void Chip8::ld_vx_i(const Opcode opcode)
	{
		for (int i = 0; i <= opcode.x(); i++)
		{
			V[i] = memory[static_cast<size_t>(I) + i];
		}
		pc += 2;
		std::cout << "instruction: Fx65 ld Vx, [I]\n";
	}
}
