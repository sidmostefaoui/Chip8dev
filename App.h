#pragma once
#include "window.h"
#include "chip8.h"
#include <memory>

namespace gui
{
	class App
	{
	public:
		static auto create(const std::string& title, int w, int h) -> void;
		static auto start_frame() -> void;
		static auto end_frame() -> void;
		static auto window() -> Window&;
		static auto running() -> bool;
		static auto input() -> emu::Keyboard;
		
	private:
		static std::unique_ptr<Window> window_;
	};

}
