#pragma once
#include <cstdint>
#include <string>

namespace Asm
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

	enum class Instruction : size_t {
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

	auto decode(const Opcode opcode) -> Instruction;

	auto disassemble(const Opcode opcode, const Instruction inst) -> std::string;



}