#include <implot.h>
#include <imgui.h>
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include <memory>

#include "PlotPanel.hpp"

namespace PPP {

    class PlotApp : public mahi::gui::Application
    {
    private:
        Axis m_PlotX;
        Axis m_PlotY;

        PlotPanel m_Panel1;

        bool gui_ShowDemo = false;

        ImGuiViewport* gui_Viewport;
        ImGuiWindowFlags gui_WindowFlags;
        ImVec2 gui_WorkAreaPos;
        ImVec2 gui_WorkAreaSize;

        Plot m_Buffer;

    public:
        PlotApp()
            :Application(960, 720, "Test")
        {
            GuiInit();

            gui_Viewport = ImGui::GetMainViewport();

            const auto min = -10.0f;
            const auto max = -min + 0.05f;

            for (double i = min ; i < max ; i += 0.05f)
            {
                m_PlotX.values.push_back(i);
                m_PlotY.values.push_back(sin(i));
            }

            m_Buffer.xAxis = std::make_shared<Axis>(m_PlotX);
            m_Buffer.yAxis = std::make_shared<Axis>(m_PlotY);
        }

        void update() override
        {
            // Multiple step process:
            //
            // 1. Panel Setup = make sure the panel exists
            //                  and it has the right data
            // 2. Data Manipulation =   The different Calculations relating to
            //                          the analysis
            // 3. GuiRender()
            m_Panel1.SubmitPlot(m_Buffer);
            GuiRender();
        }

        void GuiRender()
        {
            // Panel =  Class with both GUI and data manipulation attached to it.
            //          This type of object will change dynamically in terms of
            //          the data linked to it and
            //
            // Standard (Application) Gui will be here

            // Debug Overlay
            {
                gui_WindowFlags = ImGuiWindowFlags_NoDecoration
                    | ImGuiWindowFlags_NoDocking
                    | ImGuiWindowFlags_AlwaysAutoResize
                    | ImGuiWindowFlags_NoSavedSettings
                    | ImGuiWindowFlags_NoFocusOnAppearing
                    | ImGuiWindowFlags_NoNav
                    | ImGuiWindowFlags_NoMove;
                gui_WorkAreaPos = gui_Viewport->GetWorkPos();
                gui_WorkAreaSize = gui_Viewport->GetWorkSize();
                ImVec2 windowPos = ImVec2(
                        gui_WorkAreaPos.x + 10.0f,
                        gui_WorkAreaPos.y + gui_WorkAreaSize.y - 10.0f
                        );
                ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
                ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(gui_Viewport->ID);
                ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
                ImGui::Begin("DebugMsg", NULL, gui_WindowFlags);
                ImGui::Text("DEBUG BUILD\n");
                ImGui::Separator();
                ImGui::Text("FPS: <invalid>");
                ImGui::End();
            }

            // Dockspace and main menu bar
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    ImGui::MenuItem("New");
                    ImGui::MenuItem("Load");
                    ImGui::MenuItem("Save");
                    ImGui::MenuItem("Save As...");
                    ImGui::Separator();
                    if (ImGui::MenuItem("Quit")) Application::quit();
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem("PlotPanel", NULL, &m_Panel1.isOpen);
                    ImGui::MenuItem("Demo", NULL, &gui_ShowDemo);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            ImGui::DockSpaceOverViewport(gui_Viewport);

            m_Panel1.OnGuiRender();

            ImGui::Begin("Data");
            ImGui::Text("DATA WINDOW DUMMY");
            ImGui::Separator();

            static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                | ImGuiTableFlags_RowBg
                | ImGuiTableFlags_Resizable;

            if (ImGui::BeginTable("table", 2, flags))
            {

                ImGui::TableSetupColumn("X Axis");
                ImGui::TableSetupColumn("Y Axis");
                ImGui::TableHeadersRow();
                for (uint32_t item = 0; item < m_Buffer.xAxis->values.size(); item++)
                {
                    ImGui::PushID(item);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, 0);
                    ImGui::TableNextColumn();
                    ImGui::InputDouble("##xCell", &m_Buffer.xAxis->values[item], 0.0f, 0.0f, "%.2f");
                    ImGui::TableNextColumn();
                    ImGui::InputDouble("##yCell", &m_Buffer.yAxis->values[item], 0.0f, 0.0f, "%.2f");
                    ImGui::PopStyleColor();
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::End();

            //Demo windows
            if (gui_ShowDemo)
            {
                ImGui::ShowDemoWindow(&gui_ShowDemo);
                ImPlot::ShowDemoWindow(&gui_ShowDemo);
            }
        }

        void GuiInit()
        {
            ImVec4 *colors                         = ImGui::GetStyle().Colors;
            colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
            colors[ImGuiCol_ChildBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_PopupBg]               = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_Border]                = ImVec4(0.12f, 0.12f, 0.12f, 0.71f);
            colors[ImGuiCol_BorderShadow]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_FrameBg]               = ImVec4(0.42f, 0.42f, 0.42f, 0.54f);
            colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.42f, 0.42f, 0.42f, 0.40f);
            colors[ImGuiCol_FrameBgActive]         = ImVec4(0.56f, 0.56f, 0.56f, 0.67f);
            colors[ImGuiCol_TitleBg]               = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
            colors[ImGuiCol_TitleBgActive]         = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.17f, 0.17f, 0.17f, 0.90f);
            colors[ImGuiCol_MenuBarBg]             = ImVec4(0.335f, 0.335f, 0.335f, 1.000f);
            colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.24f, 0.24f, 0.24f, 0.53f);
            colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
            colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
            colors[ImGuiCol_CheckMark]             = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
            colors[ImGuiCol_SliderGrab]            = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
            colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
            colors[ImGuiCol_Button]                = ImVec4(0.54f, 0.54f, 0.54f, 0.35f);
            colors[ImGuiCol_ButtonHovered]         = ImVec4(0.52f, 0.52f, 0.52f, 0.59f);
            colors[ImGuiCol_ButtonActive]          = ImVec4(0.76f, 0.76f, 0.76f, 1.00f);
            colors[ImGuiCol_Header]                = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
            colors[ImGuiCol_HeaderHovered]         = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
            colors[ImGuiCol_HeaderActive]          = ImVec4(0.76f, 0.76f, 0.76f, 0.77f);
            colors[ImGuiCol_Separator]             = ImVec4(0.000f, 0.000f, 0.000f, 0.137f);
            colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.700f, 0.671f, 0.600f, 0.290f);
            colors[ImGuiCol_SeparatorActive]       = ImVec4(0.702f, 0.671f, 0.600f, 0.674f);
            colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
            colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
            colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
            colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.73f, 0.73f, 0.73f, 0.35f);
            colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
            #ifdef IMGUI_HAS_DOCK
            colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_Tab]                   = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_TabHovered]            = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
            colors[ImGuiCol_TabActive]             = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
            colors[ImGuiCol_TabUnfocused]          = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
            colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
            colors[ImGuiCol_DockingPreview]        = ImVec4(0.85f, 0.85f, 0.85f, 0.28f);
            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                colors[ImGuiCol_WindowBg].w = 1.0f;
            #endif
        }
    };
}

int main() {
    PPP::PlotApp app;
    app.run();
    return 0;
}
