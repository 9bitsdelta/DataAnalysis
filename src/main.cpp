#include <implot.h>
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include <vector>
#include <memory>
#include <imgui_demo.cpp>

#include "ViewportPanel.hpp"
#include "PlotPanel.hpp"
#include "DataPanel.hpp"
#include "imgui.h"

namespace PPP {

    class PlotApp : public mahi::gui::Application
    {
    private:

        ViewportPanel m_ViewportPanel;
        PlotPanel m_PlotPanel;
        DataPanel m_DataPanel;
        Plot m_Buffer;

        bool gui_ShowDemo = false;

        ImGuiIO& gui_IO = ImGui::GetIO();
        ImGuiViewport* gui_Viewport = ImGui::GetMainViewport();

    public:
        PlotApp()
            :Application(1280, 720, "Plot++")
        {
            GuiInit();

            const auto min = -10.0f;
            const auto max = -min + 0.05f;
            for (double i = min ; i < max ; i += 0.05f)
            {
                m_Buffer.xAxis->push_back(i);
                m_Buffer.yAxis->push_back(std::sin(mahi::util::TWOPI * 0.1f * i));
            }
            m_Buffer.xAxis->shrink_to_fit();
            m_Buffer.yAxis->shrink_to_fit();
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

            m_DataPanel.SubmitPlot(m_Buffer);
            //m_PlotPanel.SubmitPlot(m_Buffer);

            GuiRender();

            m_DataPanel.ClearPlots();
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
                auto workPos = gui_Viewport->GetWorkPos();
                auto workSize = gui_Viewport->GetWorkSize();

                static const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                    | ImGuiWindowFlags_NoDocking
                    | ImGuiWindowFlags_AlwaysAutoResize
                    | ImGuiWindowFlags_NoSavedSettings
                    | ImGuiWindowFlags_NoFocusOnAppearing
                    | ImGuiWindowFlags_NoNav
                    | ImGuiWindowFlags_NoMove;
                ImVec2 windowPos = ImVec2(
                        workPos.x + 10.0f,
                        workPos.y + workSize.y - 10.0f
                        );
                ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
                ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(gui_Viewport->ID);
                ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
                ImGui::Begin("DebugMsg", NULL, flags);
                ImGui::Text("DEBUG BUILD\n");
                ImGui::Separator();
                ImGui::Text("FPS: <invalid>");
                ImGui::End();
                ImGui::PopStyleVar();
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
                    ImGui::CheckboxFlags("io.ConfigFlags: ViewportsEnable", &gui_IO.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);

                    ImGui::MenuItem("Demo", NULL, &gui_ShowDemo);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            ImGui::DockSpaceOverViewport(gui_Viewport);

            m_ViewportPanel.OnGuiRender();
            m_PlotPanel.OnGuiRender();
            m_DataPanel.OnGuiRender();

            ShowExampleAppConsole(NULL);

            //Demo windows
            if (gui_ShowDemo)
            {
                ImGui::ShowDemoWindow(&gui_ShowDemo);
                ImPlot::ShowDemoWindow(&gui_ShowDemo);
            }
        }

        void GuiInit()
        {
            // COLORS
            ImGui::StyleColorsClassic();

            // SHAPES
            auto& style = ImGui::GetStyle();
            style.FrameRounding = 5;
            style.WindowRounding = 5;
            style.ChildRounding = 5;
            style.GrabRounding = 5;
            style.TabRounding = 5;
            style.ScrollbarRounding = 5;

            style.WindowPadding = ImVec2(4, 4);

            style.ScrollbarSize = 10;

            // IO
            gui_IO.ConfigFlags &= (~ImGuiConfigFlags_ViewportsEnable);

            gui_IO.ConfigDockingWithShift = true;
            gui_IO.ConfigViewportsNoAutoMerge = true;
            gui_IO.ConfigViewportsNoTaskBarIcon = true;
        }

    };
}

int main() {
    PPP::PlotApp app;
    app.run();
    return 0;
}
