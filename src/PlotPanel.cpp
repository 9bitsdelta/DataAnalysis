#include "PlotPanel.hpp"

#include <imgui.h>

namespace PPP {

    PlotPanel::PlotPanel()
    {

    }

    PlotPanel::~PlotPanel()
    {

    }

    void PlotPanel::OnGuiRender()
    {
        if(!ImGui::Begin("Plots", NULL, 0))
        {
            ImGui::End();
            return;
        }

        if(ImGui::BeginChild("list", ImVec2(150, 0), true))
        {
            ImGui::Text("List dummy");
        }
        ImGui::EndChild();

        ImGui::SameLine();

        if(ImGui::BeginChild("properties", ImVec2(0, 0), true))
        {
            ImGui::Text("prop dummy");

            static float colours[] = { 1.0f, 0.0f, 0.0f };
            ImGui::ColorEdit3("Color", colours);

            ImGui::Separator();

            static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                | ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable("values", 2, flags))
            {
                ImGui::TableSetupColumn("X Values");
                ImGui::TableSetupColumn("Y Values");
                ImGui::TableHeadersRow();
                for (int row_n = 0; row_n < 10; row_n++)
                {
                    ImGui::TableNextColumn();
                    ImGui::Text("Dummy");
                    ImGui::TableNextColumn();
                    ImGui::Text("Dummy");
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();

        ImGui::End();
    }

}
