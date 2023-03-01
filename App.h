#pragma once
#include "chip8.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace gui
{
	class App
	{
	public:
		static auto create(const std::string& title, int w, int h) -> void;
		static auto shutdown() -> void;
		static auto start_frame() -> void;
		static auto end_frame() -> void;
		static auto running() -> bool;
		static auto input() -> emu::Keyboard;
		
	private:
		static GLFWwindow* wnd_handle_;
	};

}
