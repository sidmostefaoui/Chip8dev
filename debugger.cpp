#include "debugger.h"
#include <fmt/format.h>

namespace chip8dev
{
	Debugger::Debugger(Chip8& vm)
		: vm(vm)
	{
	}

	auto Debugger::print_state() -> void
	{
		auto line1 = fmt::format("V0: {:#04x} V1: {:#04x} V2: {:#04x} V3: {:#04x} V4: {:#04x} V5: {:#04x}",
			vm.V[0], vm.V[1], vm.V[2], vm.V[3], vm.V[4], vm.V[5]);

		auto line2 = fmt::format("V6: {:#04x} V7: {:#04x} V8: {:#04x} V9: {:#04x} VA: {:#04x} VB: {:#04x}",
			vm.V[6], vm.V[7], vm.V[8], vm.V[9], vm.V[0xA], vm.V[0xB]);

		auto line3 = fmt::format("VC: {:#04x} VD: {:#04x} VE: {:#04x} VF: {:#04x}",
			vm.V[0xC], vm.V[0xD], vm.V[0xE], vm.V[0xF]);

		auto line4 = fmt::format("I: {:#06x} pc: {:#06x} sp: {:#04x} dt: {:#04x} st: {:#04x}",
			vm.I, vm.pc, vm.sp, vm.dt, vm.st);
	
		fmt::print("┌─{0:─^{1}}─┐\n"
				   "│ {2: ^{1}} │\n"
				   "│ {3: ^{1}} │\n"
				   "│ {4: ^{1}} │\n"
				   "│ {5: ^{1}} │\n"
			       "└─{0:─^{1}}─┘\n", "", line1.size(), line1, line2, line3, line4);
	}

	auto Debugger::print_opcode() -> void
	{
		auto op = vm.opcode_;
		fmt::print("opcode: {:#06x} x: {:#x} y: {:#x} n: {:#x} kk: {:#04x} nnn: {:#05x}\n",
			op.data(), op.x(), op.y(), op.n(), op.kk(), op.nnn());
	}

	auto Debugger::print_keyboard() -> void
	{
		fmt::print("keyboard:");
		std::string keys = "";
		for (int i = 0; i < vm.keyboard.size(); i++)
		{
			if (vm.keyboard[i] == 1)
			{
				keys = keys + fmt::format(" {:x}", i);
			}
		}
		keys = keys + "\n";
		fmt::print(keys);
	}
	

}

