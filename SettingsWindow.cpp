#include "SettingsWindow.h"
#include <imgui/imgui.h>
#include <imgui/extentions/L2DFileDialog.h>
#include <fmt/format.h>
namespace gui
{

SettingsWindow::SettingsWindow(Settings& settings, emu::Chip8& chip8)
	: chip8_(chip8), settings_(settings)
{
		::strcpy_s(file_dialog_path_, settings.rom.data());
}

auto SettingsWindow::render() -> void
{
	static bool new_rom = false;

    ImGui::Begin("Settings");
    {
		ImGui::Text(fmt::format("FPS: {}", ImGui::GetIO().Framerate).c_str());

		ImGui::ColorEdit3("color", &settings_.color.r, ImGuiColorEditFlags_NoSidePreview);
		if (ImGui::Button("select rom"))
		{
			FileDialog::file_dialog_open = true;
		}

		if (FileDialog::file_dialog_open)
		{
			FileDialog::ShowFileDialog(&FileDialog::file_dialog_open, file_dialog_path_,
				1024, FileDialog::FileDialogType::OpenFile);

			if (std::string(file_dialog_path_) != settings_.rom)
			{
				settings_.rom = file_dialog_path_;
				chip8_ = emu::Chip8(settings_.rom);
			}
		}

		ImGui::Text(fmt::format("{}", settings_.rom).c_str());
    }
    ImGui::End();
}

}