#include "SettingsWindow.h"
#include <imgui/imgui.h>
#include <imgui/extentions/L2DFileDialog.h>
#include <fmt/format.h>
namespace gui
{

SettingsWindow::SettingsWindow(Settings& settings, emu::Chip8& chip8)
	: chip8_(chip8), settings_(settings), file_dialog_path_(settings.rom)
{
}

auto SettingsWindow::render() -> void
{

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
			FileDialog::ShowFileDialog(file_dialog_path_);

			if (settings_.rom != file_dialog_path_)
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