#pragma once

#include <Mahi/Gui.hpp>

#include "Data.hpp"

namespace PPP {

    class PlotApp : public mahi::gui::Application
    {
    private:
        std::vector<Axis> m_Data;
        std::vector<Plot> m_Plots;

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
    };
}
