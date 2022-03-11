#include "ViewportPanel.hpp"
#include "imgui.h"

#include <implot.h>

namespace PPP {

    ViewportPanel::ViewportPanel()
    {
    }

    ViewportPanel::~ViewportPanel()
    {

    }

    void ViewportPanel::OnGuiRender()
    {
            if(!ImGui::Begin("Viewport", NULL, 0))
            {
                ImGui::End();
                return;
            }

            if (ImGui::Button("Fit to window")) ImPlot::FitNextPlotAxes();

            if (ImPlot::BeginPlot("Plot", "x", "y", {-1.0f, -1.0f}))
            {

            }
            ImPlot::EndPlot();
            ImGui::End();
    }

}

