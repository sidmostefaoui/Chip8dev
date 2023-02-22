#pragma once
#include"chip8.h"

namespace chip8dev
{
	class Debugger
	{
	public:
		Debugger(Chip8& vm);
		auto print_state() -> void;
		auto print_opcode() -> void;
		auto print_keyboard() -> void;

	private:
		Chip8& vm;
	};
}


