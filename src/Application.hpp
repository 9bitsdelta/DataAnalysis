#pragma once

#include <Mahi/Gui.hpp>

#include "ConsolePanel.hpp"
#include "Data.hpp"

namespace PPP {

    class PlotApp : public mahi::gui::Application
    {
    private:
        std::vector<Axis> m_Data;
        std::vector<Plot> m_Plots;
        ExampleAppConsole m_Console;

        bool gui_ShowDemo = false;

        ImGuiIO& gui_IO = ImGui::GetIO();
        ImGuiViewport* gui_Viewport = ImGui::GetMainViewport();

    public:
        PlotApp();
        ~PlotApp();

        void update() override;

        void GuiInit();
        void GuiRender();

        void ShowViewport();
        void ShowPlotPanel();
        void ShowDataPanel();

        void PlotScatter(Plot& plot);
        void PlotLine(Plot& plot);
    };
}
