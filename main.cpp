#include "chip8.h"
#include "App.h"
#include "FramebufferWindow.h"
#include "RegistersWindow.h"
#include "StackWindow.h"

#include <imgui/imgui.h>

int main()
{
    gui::App::create("CHUP8-DEV", 1280, 720);

    auto chip8 = emu::Chip8("roms/Pong.ch8");
    auto framebuffer_wnd = gui::FramebufferWindow(chip8);
    auto registers_wnd = gui::RegistersWindow(chip8);
    auto stack_wnd = gui::StackWindow(chip8);

    while (gui::App::running())
    {
        gui::App::start_frame();

        chip8.update_keyboard(gui::App::input());
        chip8.emulate_cycle();

        ImGui::Begin("Settings");
        {

            ImGui::Text("(%.1f FPS)", ImGui::GetIO().Framerate);

            ImGui::Text("Keys down:");

            for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
            {
                if (!ImGui::IsKeyDown(key))
                    continue;

                ImGui::SameLine();
                ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key);
            }
        }
        ImGui::End();

        framebuffer_wnd.render();
        registers_wnd.render();
        stack_wnd.render();

        gui::App::end_frame();
    }

    return 0;
}
