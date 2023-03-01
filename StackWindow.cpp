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

                for (int row = 0; row < chip8_.stack.size(); row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(fmt::format("{:#04x}", row).c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(fmt::format("{:#06x}", chip8_.stack[row]).c_str());
                }

                ImGui::EndTable();
            }
        }
		ImGui::End();
	}
}