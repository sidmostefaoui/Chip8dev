#pragma once

#include <string>
#include "chip8.h"

namespace gui
{
	struct RGBColor
	{
		float r;
		float g;
		float b;
	};

	struct Settings
	{
		RGBColor color;
		std::string rom;
	};

	class SettingsWindow
	{
	public:
		SettingsWindow(Settings& settings, emu::Chip8& chip_);
		auto render() -> void;
	private:
		emu::Chip8& chip8_;
		Settings& settings_;
		char file_dialog_path_[1024];
	};
}