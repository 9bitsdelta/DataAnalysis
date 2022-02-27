#include "DataPanel.hpp"

#include <imgui.h>

namespace PPP {

    DataPanel::DataPanel()
    {
    }

    DataPanel::~DataPanel()
    {

    }

    void DataPanel::OnGuiRender()
    {
            ImGui::Begin("Data", NULL, 0);
            {
                static int selected = 0;
                ImGui::BeginChild("left pane", ImVec2(150, 0), true);
                for (int i = 0; i < 25; i++)
                {
                    char label[128];
                    sprintf(label, "MyObject %d", i);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();
            {
                static int selected = 0;
                ImGui::BeginChild("second pane", ImVec2(150, 0), true);
                for (int i = 0; i < 15; i++)
                {
                    char label[128];
                    sprintf(label, "MyObject %d", i);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                }
                ImGui::EndChild();
            }
            ImGui::SameLine();

            ImGui::BeginChild("right pane", ImVec2(0, 0), true);

            static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                | ImGuiTableFlags_RowBg
                | ImGuiTableFlags_Resizable;

            if (ImGui::BeginTable("table", 2, flags))
            {

                ImGui::TableSetupColumn("X Axis");
                ImGui::TableSetupColumn("Y Axis");
                ImGui::TableHeadersRow();

                for (size_t row_n = 0; row_n < m_Plot.xAxis->size(); row_n++)
                {
                    ImGui::PushID(row_n);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                    ImGui::TableNextColumn();
                    ImGui::InputDouble("##xCell",
                            &m_Plot.xAxis[row_n],
                            0.0f,
                            0.0f,
                            "%.2f"
                            );
                    ImGui::TableNextColumn();
                    ImGui::InputDouble("##yCell",
                            &m_Plot.yAxis[row_n],
                            0.0f,
                            0.0f,
                            "%.2f"
                            );
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::EndChild();

            ImGui::End();
    }

}
