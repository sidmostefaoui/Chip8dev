#include "App.h"
#include "assert.h"

#include <glad/glad.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <fmt/format.h>
#include <stdexcept>

void glfw_error_callback(int error, const char* description)
{
	fmt::print("GLFW ERROR {}: {}\n", error, description);
	ASSERT(true);
}

void GLAPIENTRY gl_message_callback(GLenum, GLenum type, GLuint, GLenum severity,
									GLsizei, const GLchar* message, const void*)
{
	printf("GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
	ASSERT(true);
}

namespace gui
{

GLFWwindow* App::wnd_handle_ = nullptr;

void App::create(const std::string& title, int w, int h)
{
	ASSERT(w > 0);
	ASSERT(h > 0);

	if (!glfwInit())
	{
		throw std::runtime_error("Error Initializing GLFW");
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	wnd_handle_ = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);

	if (!wnd_handle_)
	{
		throw std::runtime_error("Could Not Create GLFW Window");
	}

	glfwMakeContextCurrent(wnd_handle_);
	glfwSwapInterval(0); // V-Sync

	gladLoadGL();
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(gl_message_callback, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(wnd_handle_, true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

auto App::shutdown() -> void
{
	ASSERT(wnd_handle_);
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(wnd_handle_);
	glfwTerminate();
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

	int display_w = 0;
	int display_h = 0;
	glfwGetFramebufferSize(wnd_handle_, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	static auto clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
		clear_color.z * clear_color.w, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(wnd_handle_);
}

auto App::running() -> bool
{
	ASSERT(wnd_handle_);
	return !glfwWindowShouldClose(wnd_handle_);
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

