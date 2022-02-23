#include "PlotPanel.hpp"
#include "implot.h"

#include <imgui.h>

PlotPanel::PlotPanel()
{
    isOpen = true;
}

PlotPanel::~PlotPanel()
{

}

void PlotPanel::OnGuiRender()
{
    if(isOpen)
    {
        ImGui::Begin("Viewport", &isOpen);

        if (ImGui::Button("Fit to window")) ImPlot::FitNextPlotAxes();

        ImPlot::BeginPlot("Plot", "x", "y", {-1.0f, -1.0f});
        ImPlot::PlotScatter("##", m_Plot.xAxis->values.data(), m_Plot.yAxis->values.data(), m_Plot.xAxis->values.size());
        ImPlot::EndPlot();

        ImGui::End();
    }
}
