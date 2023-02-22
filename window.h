#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <functional>
#include <string>

namespace chip8dev::gui
{

	class Window
	{
	public:
		Window(const std::string& title, int width, int height);
		~Window();

		auto should_close() -> bool;
		auto framebuffer_size() -> std::pair<int, int>;
		auto swap_buffers() -> void;

	private:
		GLFWwindow* handle;
		std::string title;
		int width;
		int height;
		ImGuiIO* io;
	};

}


