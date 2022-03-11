#pragma once

#include "Data.hpp"

namespace PPP {

    //  Shows the imported data (or generated data)
    //  Just the Axes(lists) whatever
    class DataPanel
    {
    private:
        //std::vector<Axis> m_Data;

    public:
        DataPanel();
        ~DataPanel();

        void OnGuiRender();

        void SubmitPlot(const Plot& plot) {}
        void ClearPlots() {}

        //void SubmitPlot(const Axis& plot) { m_Plots.push_back(plot); }

        //void ClearPlots() { m_Plots.clear(); }
    };

}
