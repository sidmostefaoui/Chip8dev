#include "App.h"
#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "assert.h"

namespace gui
{
	std::unique_ptr<Window> App::window_ = nullptr;

	void App::create(const std::string& title, int w, int h)
	{
		CHIP8DEV_ASSERT(w > 0);
		CHIP8DEV_ASSERT(h > 0);
		window_ = std::make_unique<Window>(title, w, h);
	}


	auto App::start_frame() -> void
	{
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	auto App::end_frame() -> void
	{
		ImGui::Render();

		auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		auto [display_w, display_h] = window_->framebuffer_size();

		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
			clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		window_->swap_buffers();
	}

	Window& App::window()
	{
		CHIP8DEV_ASSERT(window_);
		return *window_;
	}
	auto App::running() -> bool
	{
		return !window_->should_close();
	}

	/* FIX LATER: imgui reading KEY 4 as Apostrophe*/
	auto App::input() -> emu::Keyboard
	{
		/* CHIP8         PC (AZERTY)
		*  1 2 3 C       1 2 3 4
		*  4 5 6 D       A Z E R
		*  7 8 9 E       Q S D F
		*  A 0 B F       W X C V 
		*/
		emu::Keyboard keyboard = {};

		for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
		{
			if (!ImGui::IsKeyDown(key))
			{
				continue;
			}

			switch (key)
			{
			case ImGuiKey_1: keyboard[1] = true; break;
			case ImGuiKey_2: keyboard[2] = true; break;
			case ImGuiKey_3: keyboard[3] = true; break;
			case ImGuiKey_Apostrophe: keyboard[0xC] = true; break; // fix me
			case ImGuiKey_A: keyboard[4] = true; break;
			case ImGuiKey_Z: keyboard[5] = true; break;
			case ImGuiKey_E: keyboard[6] = true; break;
			case ImGuiKey_R: keyboard[0xD] = true; break;
			case ImGuiKey_Q: keyboard[7] = true; break;
			case ImGuiKey_S: keyboard[8] = true; break;
			case ImGuiKey_D: keyboard[9] = true; break;
			case ImGuiKey_F: keyboard[0xE] = true; break;
			case ImGuiKey_W: keyboard[0xA] = true; break;
			case ImGuiKey_X: keyboard[0] = true; break;
			case ImGuiKey_C: keyboard[0xB] = true; break;
			case ImGuiKey_V: keyboard[0xF] = true; break;
			default: break;
			}
		}

		return keyboard;
	}
}

