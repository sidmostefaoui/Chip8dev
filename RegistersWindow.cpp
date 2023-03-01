#include "RegistersWindow.h"
#include "imgui/imgui.h"
#include "fmt/format.h"

namespace gui
{
	RegistersWindow::RegistersWindow(const emu::Chip8& chip8)
		: chip8_(chip8)
	{
	}

	auto RegistersWindow::render() -> void
	{
		ImGui::Begin("Registers");
		
		ImGui::Text(fmt::format("v0: {:#04x}", chip8_.V[0]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("v1: {:#04x}", chip8_.V[1]).c_str());

		ImGui::Text(fmt::format("v2: {:#04x}", chip8_.V[2]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("v3: {:#04x}", chip8_.V[3]).c_str());

		ImGui::Text(fmt::format("v4: {:#04x}", chip8_.V[4]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("v5: {:#04x}", chip8_.V[5]).c_str());

		ImGui::Text(fmt::format("v6: {:#04x}", chip8_.V[6]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("v7: {:#04x}", chip8_.V[7]).c_str());

		ImGui::Text(fmt::format("v8: {:#04x}", chip8_.V[8]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("v9: {:#04x}", chip8_.V[9]).c_str());

		ImGui::Text(fmt::format("vA: {:#04x}", chip8_.V[0xA]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("vB: {:#04x}", chip8_.V[0xB]).c_str());

		ImGui::Text(fmt::format("vC: {:#04x}", chip8_.V[0xC]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("vD: {:#04x}", chip8_.V[0xD]).c_str());

		ImGui::Text(fmt::format("vE: {:#04x}", chip8_.V[0xE]).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("vF: {:#04x}", chip8_.V[0xF]).c_str());


		ImGui::Text(fmt::format("st: {:#04x}", chip8_.st).c_str());
		ImGui::SameLine();
		ImGui::Text(fmt::format("dt: {:#04x}", chip8_.dt).c_str());

		ImGui::Text(fmt::format("I: {:#06x}", chip8_.I).c_str());

		ImGui::Text(fmt::format("pc: {:#06x}", chip8_.pc).c_str());

		ImGui::Text(fmt::format("sp: {:#04x}", chip8_.sp).c_str());


		ImGui::End();
	}
}