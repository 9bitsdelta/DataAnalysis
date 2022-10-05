#include "Application.hpp"
#include "Data.hpp"
#include "imgui.h"

#include <Mahi/Util.hpp>

#include <imgui_demo.cpp> //TODO: remove this

#define AUTO_COL ImPlot::GetColormapColor(m_Plots.size())

namespace PPP {

    const Axis empty;
    Axis Time, Sine, Line, Regressed;

    PlotApp::PlotApp()
        :Application(1280, 720, "Plot++")
    {
        GuiInit();

        mahi::util::set_random_seed(0);

        m_Data.reserve(10);
        m_Plots.reserve(10);

        Time.label = "Time";
        Sine.label = "Sine";
        Line.label = "Line";
        Regressed.label = "Regressed";

        const float min = -10.0f;
        const float max = -min + 0.05f;

        const float max_err = 0.5f;

        for(float i = min ; i < max ; i += 0.05f)
        {
            Time->push_back(i);
            Sine->push_back(5 + 10 * std::sin(0.2f * 3.14f * i)
                    + mahi::util::random_range(-max_err, max_err));
            Line->push_back(i + mahi::util::random_range(-max_err, max_err));
        }

        float a, b;
        mahi::util::linear_regression(*Time.get(), *Line.get(), a, b);
        m_Console.AddLog("y = %f * x + %f\n", a, b);

        Regressed->reserve(Time->size());
        for(size_t i = 0; i < Time->size(); i++)
            Regressed->push_back(a * Time[i] + b);

        m_Data.push_back(Time);
        m_Data.push_back(Sine);
        m_Data.push_back(Line);
        m_Data.push_back(Regressed);


        m_Plots.emplace_back("Sine", Time, Sine, AUTO_COL);
        m_Plots.emplace_back("Line", Time, Line, AUTO_COL);
        m_Plots.emplace_back("Regressed", Time, Regressed, AUTO_COL, PlotType::Line);
    }

    PlotApp::~PlotApp() {}

    void PlotApp::update()
    {
        GuiRender();
    }

    ////////////////////////////////////////////////////////////////////////////
    // GUI
    ////////////////////////////////////////////////////////////////////////////

    void PlotApp::GuiInit()
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

    void PlotApp::GuiRender()
    {
        // Debug Overlay
        {
            auto workPos = gui_Viewport->GetWorkPos();
            auto workSize = gui_Viewport->GetWorkSize();

            static const ImGuiWindowFlags flags =
                  ImGuiWindowFlags_NoDecoration
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
            ImGui::SetNextWindowPos(windowPos,
                    ImGuiCond_Always,
                    window_pos_pivot);
            ImGui::SetNextWindowViewport(gui_Viewport->ID);
            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
            ImGui::Begin("DebugMsg", NULL, flags);
            ImGui::Text("DEBUG BUILD\n");
            ImGui::Separator();
            ImGui::End();
            ImGui::PopStyleVar();
        }

        // Dockspace and main menu bar
        if(ImGui::BeginMainMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Load");
                ImGui::MenuItem("Save");
                ImGui::MenuItem("Save As...");
                ImGui::Separator();
                ImGui::MenuItem("Import Data");
                ImGui::Separator();
                if(ImGui::MenuItem("Quit")) Application::quit();
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("Window"))
            {
                ImGui::CheckboxFlags("io.ConfigFlags: ViewportsEnable",
                        &gui_IO.ConfigFlags,
                        ImGuiConfigFlags_ViewportsEnable);
                ImGui::MenuItem("Demo", NULL, &gui_ShowDemo);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::DockSpaceOverViewport(gui_Viewport);

        //Panels
        ShowViewport();
        ShowPlotPanel();
        ShowDataPanel();

        m_Console.Draw("Example: Console", nullptr);

        //Demo windows
        if(gui_ShowDemo)
        {
            ImGui::ShowDemoWindow(&gui_ShowDemo);
            ImPlot::ShowDemoWindow(&gui_ShowDemo);
        }
    }

    ////////////////////////////////////////////////////////////////////////////
    // VIEWPORT PANEL
    ////////////////////////////////////////////////////////////////////////////

    void PlotApp::ShowViewport()
    {
        if(ImGui::Begin("Viewport"))
        {
            if(ImPlot::BeginPlot("Plot", "x", "y", {-1.0f, -1.0f}))
            {
                for(auto& plot : m_Plots)
                {
                    ImGui::PushID(&plot);
                    if(plot.type == PlotType::Scatter)
                        PlotScatter(plot);
                    else if(plot.type == PlotType::Line)
                        PlotLine(plot);
                    ImGui::PopID();
                }
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    void PlotApp::PlotScatter(Plot& plot)
    {
        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle,
                                   IMPLOT_AUTO,
                                   plot.colour,
                                   IMPLOT_AUTO,
                                   ImVec4(plot.colour.x,
                                          plot.colour.y,
                                          plot.colour.z,
                                          1)
                                   );
        auto size = std::min(plot.xAxis->size(), plot.yAxis->size());
        ImPlot::PlotScatter(plot.title.c_str(),
                            plot.xAxis->data(),
                            plot.yAxis->data(),
                            size
                            );
    }

    void PlotApp::PlotLine(Plot& plot)
    {
        auto size = std::min(plot.xAxis->size(), plot.yAxis->size());
        ImPlot::SetNextLineStyle(plot.colour);
        ImPlot::PlotLine(plot.title.c_str(),
                         plot.xAxis->data(),
                         plot.yAxis->data(),
                         size
                         );
    }

    ////////////////////////////////////////////////////////////////////////////
    // PLOT PROPERTY PANEL
    ////////////////////////////////////////////////////////////////////////////

    void PlotApp::ShowPlotPanel()
    {
        if(ImGui::Begin("Plots", NULL, 0))
        {
            static size_t s_SelIdx = 0;

            if(m_Plots.empty())
            {
                m_Plots.emplace_back("New Plot", empty, empty, AUTO_COL, PlotType::Scatter);
            }

            if(ImGui::Button("Add Plot"))
                m_Plots.emplace_back("New Plot", empty, empty, AUTO_COL, PlotType::Scatter);

            //ImGui::SameLine();

            //if(ImGui::Button("Delete Selected Plot"))
            //{
            //    m_Plots.erase(m_Plots.begin() + s_SelIdx);

            //    std::cout << "Selected = " << s_SelIdx << "\n";
            //    std::cout << "m_Plts.size = " << m_Plots.size() << "\n";
            //    std::cout << "m_Plots.begin = " << &*m_Plots.begin() << "\n";
            //    std::cout << "m_Plots.end = " << &*m_Plots.end() << "\n";
            //    //FIXME: SOMETHING IS SERIOUSLY MESSED UP HERE
            //}

            if(ImGui::BeginChild("list", ImVec2(128, 0), true))
            {

                for(size_t i = 0; i < m_Plots.size(); i++)
                {
                    ImGui::PushID(i);
                    ImPlot::ItemIcon(ImVec4(
                                m_Plots[i].colour.x,
                                m_Plots[i].colour.y,
                                m_Plots[i].colour.z,
                                1));
                    ImGui::SameLine();
                    if(ImGui::Selectable(m_Plots[i].title.c_str(),
                                s_SelIdx == i))
                    {
                        s_SelIdx = i;
                        //m_Console.AddLog("Selected: %lu", s_SelIdx);
                        //m_Console.AddLog("m_Plts.size() = %lu", m_Plots.size());
                    }
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            if(ImGui::BeginChild("properties", ImVec2(0, 0), true))
            {
                auto& plot = m_Plots[s_SelIdx];

                ImGui::InputText("Title", &plot.title);

                ImGui::Separator();

                ImGui::ColorEdit4("Color", &plot.colour.x);

                ImGui::Separator();

                const char* plotTypes[] = { "Scatter", "Line" };
                ImGui::Combo("Plot Type", (int*)&plot.type, plotTypes, IM_ARRAYSIZE(plotTypes));

                ImGui::Separator();

                static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                    | ImGuiTableFlags_RowBg;
                if(ImGui::BeginTable("values", 2, flags))
                {
                    ImGui::TableSetupColumn("X Values");
                    ImGui::TableSetupColumn("Y Values");
                    ImGui::TableHeadersRow();
                    ImGui::TableNextColumn();
                    ImGui::Selectable(plot.xAxis.get_label());
                    if(ImGui::BeginDragDropTarget()) {
                        if(const ImGuiPayload* payload =
                                ImGui::AcceptDragDropPayload("DND"))
                        {
                            plot.xAxis = *(Axis*)payload->Data;
                        }
                        ImGui::EndDragDropTarget();
                    }
                    ImGui::TableNextColumn();
                    ImGui::Selectable(plot.yAxis.get_label());
                    if(ImGui::BeginDragDropTarget()) {
                        if(const ImGuiPayload* payload =
                                ImGui::AcceptDragDropPayload("DND"))
                        {
                            plot.yAxis = *(Axis*)payload->Data;
                        }
                        ImGui::EndDragDropTarget();
                    }
                    for(size_t row_n = 0;
                        row_n < plot.xAxis->size();
                        row_n++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text("%.3f", plot.xAxis[row_n]);

                        ImGui::TableNextColumn();
                        ImGui::Text("%.3f", plot.yAxis[row_n]);
                    }
                }
                ImGui::EndTable();
            }
            ImGui::EndChild();
        }
        ImGui::End();
    }

    ////////////////////////////////////////////////////////////////////////////
    // DATA PANEL
    ////////////////////////////////////////////////////////////////////////////

    void PlotApp::ShowDataPanel()
    {
        if(ImGui::Begin("Data", NULL, 0))
        {
            static ImGuiTableFlags tableFlags = ImGuiTableFlags_Borders
                                              | ImGuiTableFlags_RowBg
                                              | ImGuiTableFlags_Resizable;

            if(ImGui::BeginTable("table", 2, tableFlags))
            {
                ImGui::TableSetupColumn("Label");
                ImGui::TableSetupColumn("Values");
                ImGui::TableHeadersRow();

                for(auto& axis : m_Data)
                {
                    ImGui::TableNextColumn();
                    ImGui::Selectable(axis.get_label());
                    if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
                    {
                        ImGui::SetDragDropPayload("DND", &axis, sizeof(Axis));
                        ImGui::TextUnformatted(axis.get_label());
                        ImGui::EndDragDropSource();
                    }
                    //if(ImGui::BeginDragDropTarget()) {
                    //    if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND"))
                    //    {
                    //    }
                    //    ImGui::EndDragDropTarget();
                    //}
                    ImGui::TableNextColumn();
                    ImGui::Text("[%.3f, %.3f, %.3f ...",
                            axis[0],
                            axis[1],
                            axis[2]
                            );
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }
}
