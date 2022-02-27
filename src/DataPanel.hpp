#pragma once

#include "Plot.hpp"

namespace PPP {

    class DataPanel
    {
    private:
        Plot m_Plot;

    public:
        DataPanel();
        ~DataPanel();

        void OnGuiRender();

        void SubmitPlot(const Plot& plot) { m_Plot = plot; }
    };

}
