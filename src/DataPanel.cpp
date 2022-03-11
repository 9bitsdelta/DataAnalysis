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
        //const float width = 150;

        if(!ImGui::Begin("Data", NULL, 0))
        {
            ImGui::End();
            return;
        }

        //ImGui::BeginGroup();
        //ImGui::Text("AXES");
        //if (ImGui::BeginChild("axes",
        //            ImVec2(width, 0),
        //            true))
        //{
        //    static Axis<double>* selected = NULL;
        //    for (auto& i : m_Plots)
        //    {
        //        char xLabel[128];
        //        char yLabel[128];

        //        sprintf(xLabel, "%s", i.xAxis.label.c_str());
        //        sprintf(yLabel, "%s", i.yAxis.label.c_str());

        //        ImGui::PushID(&i.xAxis);
        //        if (ImGui::Selectable(xLabel, selected == &i.xAxis))
        //            selected = &i.xAxis;
        //        ImGui::PopID();

        //        ImGui::PushID(&i.yAxis);
        //        if (ImGui::Selectable(yLabel, selected == &i.yAxis))
        //            selected = &i.yAxis;
        //        ImGui::PopID();
        //    }
        //}
        //ImGui::EndChild();
        //ImGui::EndGroup();

        //ImGui::SameLine();

        //ImGui::BeginGroup();
        //ImGui::Text("PLOTS");
        //if (ImGui::BeginChild("plots",
        //            ImVec2(width, 0),
        //            true))
        //{
        //    static int selected = 0;
        //    for (int i = 0; i < 15; i++)
        //    {
        //        char label[128];
        //        sprintf(label, "MyObject %d", i);
        //        if (ImGui::Selectable(label, selected == i))
        //            selected = i;
        //    }
        //}
        //ImGui::EndChild();
        //ImGui::EndGroup();

        //ImGui::SameLine();

        //ImGui::BeginGroup();
        //ImGui::Text("Details");
        //ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        //if (ImGui::BeginChild("data_table", ImVec2(0, 0), false))
        //{
            static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                | ImGuiTableFlags_RowBg
                | ImGuiTableFlags_Resizable;

            if (ImGui::BeginTable("table", 3, flags))
            {

                ImGui::TableSetupColumn("Label");
                ImGui::TableSetupColumn("Type");
                ImGui::TableSetupColumn("Values");
                ImGui::TableHeadersRow();

                for (size_t row_n = 0; row_n < 10; row_n++)
                {
                    ImGui::PushID(row_n);
                    ImGui::TableNextColumn();
                    ImGui::Text("Dummy 1");
                    ImGui::TableNextColumn();
                    ImGui::Text("Dummy 2");
                    ImGui::TableNextColumn();
                    ImGui::Text("Dummy 3");
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        //}
        //ImGui::EndChild();
        //ImGui::PopStyleVar();
        //ImGui::EndGroup();

        ImGui::End();
    }

}
