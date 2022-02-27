#include "PlotPanel.hpp"
#include "imgui.h"

#include <implot.h>

namespace PPP {

    PlotPanel::PlotPanel()
    {
    }

    PlotPanel::~PlotPanel()
    {

    }

    void PlotPanel::OnGuiRender()
    {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Begin("Viewport", NULL, 0);
            if (ImGui::Button("Fit to window")) ImPlot::FitNextPlotAxes();


            if (ImPlot::BeginPlot("Plot", "x", "y", {-1.0f, -1.0f}))
            {
                ImPlot::PlotScatter("##",
                        m_Plot.xAxis->data(),
                        m_Plot.yAxis->data(),
                        m_Plot.xAxis->size()
                        );
                ImPlot::EndPlot();
            }
            ImGui::End();
            ImGui::PopStyleVar();
    }

}

