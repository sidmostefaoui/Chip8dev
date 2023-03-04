#include "chip8.h"
#include "App.h"
#include "FramebufferWindow.h"
#include "RegistersWindow.h"
#include "StackWindow.h"
#include "SettingsWindow.h"

int main()
{
    gui::App::create("CHUP8-DEV", 1280, 720);

    gui::Settings settings = { {255.0f, 255.0f, 255.0f}, "roms\\trip8.ch8" };
    auto chip8 = emu::Chip8(settings.rom);

    auto framebuffer_wnd = gui::FramebufferWindow(chip8, settings);
    auto registers_wnd = gui::RegistersWindow(chip8);
    auto stack_wnd = gui::StackWindow(chip8);
    auto settings_wnd = gui::SettingsWindow(settings, chip8);

    while (gui::App::running())
    {
        gui::App::start_frame();

        chip8.update_keyboard(gui::App::input());
        chip8.emulate_cycle();

        framebuffer_wnd.render();
        registers_wnd.render();
        stack_wnd.render();
        settings_wnd.render();

        gui::App::end_frame();
    }

    gui::App::shutdown();

    return 0;
}
