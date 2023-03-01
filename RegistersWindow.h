#pragma once
#include "chip8.h"

namespace gui
{
	class RegistersWindow
	{
	public:
		RegistersWindow(const emu::Chip8& chip8);
		auto render() -> void;
	private:
		const emu::Chip8& chip8_;
	};

}