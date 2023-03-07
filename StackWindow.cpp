#include "StackWindow.h"
#include "imgui/imgui.h"

#include "fmt/format.h"

namespace gui
{

	StackWindow::StackWindow(const emu::Chip8& chip8)
		: chip8_(chip8)
	{
	}

	auto StackWindow::render() -> void
	{
		ImGui::Begin("Stack");
        {
            if(ImGui::BeginTable("table1", 2, ImGuiTableFlags_Borders))
            {
                ImGui::TableSetupColumn("adress");
                ImGui::TableSetupColumn("value");
                ImGui::TableHeadersRow();

                for (uint8_t row = 0; row < 16; row++)
                {
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    auto adr = 0x50 + row * 2;
                    static auto adr_str = std::string(10, ' ');
                    fmt::format_to(adr_str.begin(), "{:#04x}", adr);
                    ImGui::Text(adr_str.c_str());

                    ImGui::TableSetColumnIndex(1);
                    uint16_t hi = chip8_.memory[adr];
                    uint16_t lo = chip8_.memory[static_cast<size_t>(adr)];
                    uint16_t val = (hi << 8) | lo;
                    static auto val_str = std::string(10, ' ');
                    fmt::format_to(val_str.begin(), "{:#06x}", val);
                    ImGui::Text(val_str.c_str());
                }

                ImGui::EndTable();
            }
        }
		ImGui::End();
	}
}