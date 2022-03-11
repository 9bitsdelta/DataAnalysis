#pragma once

#include "Data.hpp"

// Wishlist
//      - Viewports are just a string
//      - Axes are objects (ordered arrays of certain data types + label + quick access to first and last)
//      - Plots are pairs of axes (shared ptr), a plot type, and a viewport
//      - Plots and Axes are stored in lists, drag and drop axes to create plots
//      - PlotPanel displays all plots (the list) and the graphics in their own viewports
//      - Drag and drop plot to viewport (for visual)

namespace PPP {

    class ViewportPanel
    {
    private:
        Plot m_Plot;

    public:
        ViewportPanel();
        ~ViewportPanel();

        void OnGuiRender();

        void SubmitPlot(const Plot& plot) { m_Plot = plot; }
    };

}

