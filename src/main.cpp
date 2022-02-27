#include <implot.h>
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include <vector>
#include <memory>

#include "PlotPanel.hpp"
#include "DataPanel.hpp"
#include "imgui.h"

namespace PPP {

    class PlotApp : public mahi::gui::Application
    {
    private:

        PlotPanel m_PlotPanel;
        DataPanel m_DataPanel;
        Plot m_Buffer;

        bool gui_ShowDemo = false;

        ImGuiViewport* gui_Viewport;
        ImVec2 gui_WorkAreaPos;
        ImVec2 gui_WorkAreaSize;

    public:
        PlotApp()
            :Application(960, 720, "Plot++")
        {
            GuiInit();

            gui_Viewport = ImGui::GetMainViewport();

            const auto min = -10.0f;
            const auto max = -min + 0.05f;
            for (double i = min ; i < max ; i += 0.05f)
            {
                m_Buffer.xAxis->push_back(i);
                m_Buffer.yAxis->push_back(std::sin(mahi::util::TWOPI * 0.2f * i));
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
            m_PlotPanel.SubmitPlot(m_Buffer);

            GuiRender();
        }

        void GuiRender()
        {
            gui_WorkAreaPos = gui_Viewport->GetWorkPos();
            gui_WorkAreaSize = gui_Viewport->GetWorkSize();

            // Panel =  Class with both GUI and data manipulation attached to it.
            //          This type of object will change dynamically in terms of
            //          the data linked to it and
            //
            // Standard (Application) Gui will be here

            // Debug Overlay
            {
                static const ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                    | ImGuiWindowFlags_NoDocking
                    | ImGuiWindowFlags_AlwaysAutoResize
                    | ImGuiWindowFlags_NoSavedSettings
                    | ImGuiWindowFlags_NoFocusOnAppearing
                    | ImGuiWindowFlags_NoNav
                    | ImGuiWindowFlags_NoMove;
                ImVec2 windowPos = ImVec2(
                        gui_WorkAreaPos.x + 10.0f,
                        gui_WorkAreaPos.y + gui_WorkAreaSize.y - 10.0f
                        );
                ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
                ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(gui_Viewport->ID);
                ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
                ImGui::Begin("DebugMsg", NULL, flags);
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
                    ImGui::MenuItem("Demo", NULL, &gui_ShowDemo);
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            ImGui::DockSpaceOverViewport(gui_Viewport);

            m_PlotPanel.OnGuiRender();
            m_DataPanel.OnGuiRender();

            //Demo windows
            if (gui_ShowDemo)
            {
                ImGui::ShowDemoWindow(&gui_ShowDemo);
                ImPlot::ShowDemoWindow(&gui_ShowDemo);
            }
        }

        void GuiInit()
        {
            ImGui::StyleColorsClassic();

            auto& style = ImGui::GetStyle();
            style.FrameRounding = 5.0f;
            style.WindowRounding = 5.0f;
        }

    };
}

int main() {
    PPP::PlotApp app;
    app.run();
    return 0;
}
