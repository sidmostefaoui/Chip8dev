
#include <iostream>
#include <algorithm>
#include <cassert>
#include "chip8.h"
#include "debugger.h"

#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "window.h"
#include "gl.h"

auto GetPressedKeys() -> chip8dev::Keyboard;
auto DrawFrameBuffer(chip8dev::Framebuffer framebuffer, int zoom) -> void;

int main()
{
    
    const int zoom = 8;
    const int width = 64 * zoom;
    const int height = 32 * zoom;
    
    auto window = chip8dev::gui::Window("CHIP8-DEV", width, height);
    auto chip8 = chip8dev::Chip8("roms/Pong.ch8");

    while (!window.should_close())
    {
        glfwPollEvents();
        chip8.emulate_cycle();
       
        auto tex = chip8dev::Texture::from_framebuffer(chip8.framebuffer());


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("OpenGL Texture Text");
        {
            ImGui::Text("pointer = %p", tex.id);
            ImGui::Text("size = %d x %d", tex.w, tex.h);
            ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

            ImGui::Image((void*)(intptr_t)tex.id, ImVec2((float)tex.w * 8,
                (float)tex.h * 8));
        }
        ImGui::End();

      

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


        ImGui::Render();

        auto [display_w, display_h] = window.framebuffer_size();

        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swap_buffers();
    }
   /* const int zoom = 8;
    const int screenWidth = 64 * zoom;
    const int screenHeight = 32 * zoom;

    InitWindow(screenWidth, screenHeight, "CHIP-8");
    SetTargetFPS(60);
    
    auto chip8 = chip8emu::chip8("roms/Soccer.ch8");
    auto debugger = chip8emu::debugger(chip8);

    while (!WindowShouldClose())
    {
        chip8.update_pressed_keys(GetPressedKeys());
        chip8.emulate_cycle();
        debugger.print_opcode();
        debugger.print_keyboard();
        debugger.print_state();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawFrameBuffer(chip8.framebuffer_data(), zoom);
        EndDrawing();
    }

    CloseWindow();*/

    return 0;
}


//auto GetPressedKeys() -> chip8emu::keyboard_t {
//    std::array<bool, 16> keys = {};
//
//    static constexpr std::array<int, 16> pc_keyboard = {
//        KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,
//        KEY_Q, KEY_W, KEY_E, KEY_R,
//        KEY_A, KEY_S, KEY_D, KEY_F,
//        KEY_Z, KEY_X, KEY_C, KEY_V
//    };
//
//    static constexpr std::array<int, 16> chip8_keyboard = {
//        1, 2, 3, 0xC,
//        4, 5, 6, 0xD,
//        7, 8, 9, 0xE,
//        0xA, 0, 0xB, 0xF
//    };
//
//    using namespace std;
//
//    auto exists_in_chip8 = [](int key) {
//        if (find(begin(pc_keyboard), end(pc_keyboard), key) != end(pc_keyboard))
//            return true;
//        return false;
//    };
//
//    auto map_to_chip8 = [](int key) {
//        auto pos = find(begin(pc_keyboard), end(pc_keyboard), key) - begin(pc_keyboard);
//        return chip8_keyboard[pos];
//
//    };
//
//    while (auto key = GetKeyPressed())
//        if (exists_in_chip8(key))
//            keys[map_to_chip8(key)] = true;
//            
//    return keys;
//}
//
//void DrawPixel(int x, int y, int zoom) {
//    assert(x >= 0 && y >= 0 && zoom >= 1);
//    DrawRectangle(x * zoom, y * zoom, zoom, zoom, WHITE);
//}
//
//auto DrawFrameBuffer(chip8emu::framebuffer_t framebuffer, int zoom) -> void
//{
//    for (size_t row = 0; row < 32; row++)
//    {
//        for (size_t col = 0; col < 64; col++)
//        {
//            if (framebuffer[row][col] == true)
//            {
//                DrawPixel(static_cast<int>(col), static_cast<int>(row), zoom);
//            }
//        }
//    }
//}


