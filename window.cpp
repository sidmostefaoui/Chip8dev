#include "window.h"
#include <stdexcept>
#include <glad/glad.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <fmt/format.h>
#include "gl.h"

namespace gui
{

    void glfw_error_callback(int error, const char* description)
    {
        fmt::print("GLFW ERROR {}: {}\n", error, description);
    }

    Window::Window(const std::string& title, int width, int height)
        : title(title), width(width), height(height)
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Error Initializing GLFW");
        }

        glfwSetErrorCallback(glfw_error_callback);

        const char* glsl_version = "#version 130";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (!handle)
        {
            throw std::exception("Could Not Create GLFW Window");
        }

        glfwMakeContextCurrent(handle);
        glfwSwapInterval(0); // V-Sync

        gladLoadGL();
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(gl::MessageCallback, 0);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(handle, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    Window::~Window()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(handle);
        glfwTerminate();
    }

    auto Window::should_close() -> bool
    {
        return glfwWindowShouldClose(handle);
    }

    auto Window::framebuffer_size() -> std::pair<int, int>
    {
        std::pair<int, int > size;
        glfwGetFramebufferSize(handle, &size.first, &size.second);
        return size;
    }
    auto Window::swap_buffers() -> void
    {
        glfwSwapBuffers(handle);
    }

}

