#pragma once
#include "chip8.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <miniaudio/miniaudio.h>


namespace gui
{
	class App
	{
	public:
		static auto create(const std::string& title, int w, int h) -> void;
		static auto shutdown() -> void;
		static auto start_frame() -> void;
		static auto end_frame() -> void;
		static auto is_running() -> bool;
		static auto delta_time() -> float;
		static auto input() -> emu::Keyboard;
		static auto beep() -> void;
		
	private:
		static GLFWwindow* wnd_handle_;
		static ma_engine audio_engine_;
	};

}
