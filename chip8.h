#pragma once

#include<array>
#include<string>

namespace chip8dev
{

struct Opcode // chip8 opcodes are 2 bytes long
{
	uint8_t hi; // high byte (big endian)
	uint8_t lo;  // low byte (big endian)

	uint8_t hi_left() const { return (hi & 0xF0) >> 4; } // leftmost 4 bits of high byte
	uint8_t hi_right() const { return (hi & 0x0F); }     // rightmost 4 bits of high byte
	uint8_t lo_left() const { return (lo & 0xF0) >> 4; } // leftmost 4 bits of low byte
	uint8_t lo_right() const { return (lo & 0x0F); }     // rightmost 4 bits of low byte
	uint8_t x() const { return hi_right(); }
	uint8_t y() const { return lo_left(); }
	uint8_t n() const { return lo_right(); }
	uint8_t kk() const { return lo; }

	uint16_t data() const // return opcode as single 16bit value
	{
		uint16_t hi_16 = static_cast<uint16_t>(hi);
		uint16_t lo_16 = static_cast<uint16_t>(lo);
		return  (hi_16 << 8) | lo_16;
	}

	uint16_t nnn() const
	{
		uint16_t hi_right_16 = static_cast<uint16_t>(hi_right());
		uint16_t lo_16 = static_cast<uint16_t>(lo);
		return (hi_right_16 << 8) | lo_16;
	}
};

enum class Instruction;
using Framebuffer = std::array<std::array<uint8_t, 64>, 32>;
using Keyboard = std::array<bool, 16>;


class Chip8
{
public:
	Chip8(const std::string& rom);
	void emulate_cycle();
	void update_pressed_keys(const Keyboard& keys);
	const Framebuffer& framebuffer() const;

private:
	friend class Debugger;
	/* mapping binary opcode code to instructions */
	Opcode fetch();
	Instruction decode(const Opcode opcode);
	void execute(const Instruction inst, const Opcode op);

	/* instruction set */
	void cls(const Opcode opcode); 
	void ret(const Opcode opcode);
	void jp(const Opcode opcode);
	void call_nnn(const Opcode opcode);
	void se_vx_kk(const Opcode opcode);
	void sne_vx_kk(const Opcode opcode);
	void se_vx_vy(const Opcode opcode);
	void ld_vx_kk(const Opcode opcode);
	void add_vx_kk(const Opcode opcode);
	void sub_vx_vy(const Opcode opcode);
	void shr_vx(const Opcode opcode);
	void subn_vx_vy(const Opcode opcode);
	void shl_vx(const Opcode opcode);
	void sne_vx_vy(const Opcode opcode);
	void ld_i_nnn(const Opcode opcode);
	void jp_v0_nnn(const Opcode opcode);
	void rnd_vx_kk(const Opcode opcode);
	void drw_vx_vy(const Opcode opcode);
	void skp_vx(const Opcode opcode);
	void sknp_vx(const Opcode opcode);
	void ld_vx_dt(const Opcode opcode);
	void add_i_vx(const Opcode opcode);
	void ld_f_vx(const Opcode opcode);
	void ld_b_vx(const Opcode opcode);
	void ld_i_vx(const Opcode opcode);
	void ld_vx_i(const Opcode opcode);
	void ld_vx_k(const Opcode opcode);
	void ld_dt_vx(const Opcode opcode);
	void ld_st_vx(const Opcode opcode);
	void ld_vx_vy(const Opcode opcode);
	void or_vx_vy(const Opcode opcode);
	void and_vx_vy(const Opcode opcode);
	void xor_vx_vy(const Opcode opcode);
	void add_vx_vy(const Opcode opcode);

	/* virtual machine internal state */
	std::array<uint8_t, 4096> memory;
	std::array<uint16_t, 16> stack;
	std::array<uint8_t, 16> V; // general purpose registers
	uint16_t I; // register used to store memory adresses
	uint16_t pc; // program counter
	uint8_t sp; // stack pointer
	uint8_t st; // sound timer register
	uint8_t dt; // delay timer register
	Opcode opcode_;
	Keyboard keyboard;
	Framebuffer framebuffer_;
};

}



