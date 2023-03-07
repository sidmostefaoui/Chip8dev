#include "asm.h"
#include "Assert.h"
#include <fmt/format.h>
#include <stdexcept>

namespace Asm
{
	auto decode(const Opcode opcode) -> Instruction
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

	auto disassemble(const Opcode opcode, const Instruction inst) -> std::string
	{
		std::string disassembly;

		switch (inst)
		{
		case Instruction::_00E0:
			disassembly =  "cls";
			break;
		case Instruction::_00EE:
			disassembly =  "ret";
			break;
		case Instruction::_1NNN:
			disassembly =  fmt::format("jp {}", opcode.nnn());
			break;
		case Instruction::_2NNN:
			disassembly =  fmt::format("call {}", opcode.nnn());
			break;
		case Instruction::_3XKK:
			disassembly =  fmt::format("se V{}, {}", opcode.x(), opcode.kk());
			break;
		case Instruction::_4XKK:
			disassembly =  fmt::format("sne V{}, {}", opcode.x(), opcode.kk());
			break;
		case Instruction::_5XY0:
			disassembly =  fmt::format("se V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_6XKK:
			disassembly =  fmt::format("ld V{}, {}", opcode.x(), opcode.kk());
			break;
		case Instruction::_7XKK:
			disassembly =  fmt::format("add V{}, {}", opcode.x(), opcode.kk());
			break;
		case Instruction::_8XY0:
			disassembly =  fmt::format("ld V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY1:
			disassembly =  fmt::format("or V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY2:
			disassembly =  fmt::format("and V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY3:
			disassembly =  fmt::format("xor V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY4:
			disassembly =  fmt::format("add V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY5:
			disassembly =  fmt::format("sub V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XY6:
			disassembly =  fmt::format("shr V{}", opcode.x());
			break;
		case Instruction::_8XY7:
			disassembly =  fmt::format("subn V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_8XYE:
			disassembly =  fmt::format("shl V{}", opcode.x());
			break;
		case Instruction::_9XY0:
			disassembly =  fmt::format("sne V{}, V{}", opcode.x(), opcode.y());
			break;
		case Instruction::_ANNN:
			disassembly =  fmt::format("ld I, {}", opcode.nnn());
			break;
		case Instruction::_BNNN:
			disassembly =  fmt::format("jp V0, {}", opcode.nnn());
			break;
		case Instruction::_CXKK:
			disassembly =  fmt::format("rnd V{}, {}", opcode.x(), opcode.kk());
			break;
		case Instruction::_DXYN:
			disassembly =  fmt::format("drw V{}, V{}, {}", opcode.x(), opcode.y(), opcode.n());
			break;
		case Instruction::_EX9E:
			disassembly =  fmt::format("skp V{}", opcode.x());
			break;
		case Instruction::_EXA1:
			disassembly =  fmt::format("sknp V{}", opcode.x());
			break;
		case Instruction::_FX07:
			disassembly =  fmt::format("ld V{}, dt", opcode.x());
			break;
		case Instruction::_FX0A:
			disassembly =  fmt::format("ld V{}, key", opcode.x());
			break;
		case Instruction::_FX15:
			disassembly =  fmt::format("ld dt, V{}", opcode.x());
			break;
		case Instruction::_FX18:
			disassembly =  fmt::format("ld st, V{}", opcode.x());
			break;
		case Instruction::_FX1E:
			disassembly =  fmt::format("add I, V{}", opcode.x());
			break;
		case Instruction::_FX29:
			disassembly =  fmt::format("ld F, V{}", opcode.x());
			break;
		case Instruction::_FX33:
			disassembly =  fmt::format("ld B, V{}", opcode.x());
			break;
		case Instruction::_FX55:
			disassembly =  fmt::format("ld [I], V{}", opcode.x());
			break;
		case Instruction::_FX65:
			disassembly =  fmt::format("ld V{}, [I]", opcode.x());
			break;
		default:
			ASSERT(false);
		}

		return disassembly;
	}
}