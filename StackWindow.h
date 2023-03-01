#pragma once
#include "chip8.h"

namespace gui
{
	class StackWindow
	{
	public:
		StackWindow(const emu::Chip8& chip8);
		virtual auto render() -> void;
	private:
		const emu::Chip8& chip8_;
	};

}