#include "Application.hpp"

#include <Mahi/Util.hpp>

#include <imgui_demo.cpp> //TODO: remove this

namespace PPP {

    Axis Time, Sine, Line;

    PlotApp::PlotApp()
        :Application(1280, 720, "Plot++")
    {
        GuiInit();

        m_Data.reserve(3);
        m_Plots.reserve(2);

        const double min = -10.0f;
        const double max = -min + 0.05f;
        for(float i = min ; i < max ; i += 0.05f)
        {
            Time->push_back(i);
            Sine->push_back(10 * std::sin(0.2f * 3.14f * i));
            Line->push_back(i);
        }

        m_Data.push_back(Time);
        m_Data.push_back(Sine);
        m_Data.push_back(Line);

        Plot A("Sine",
               Time,
               Sine,
               ImPlot::GetColormapColor(m_Plots.size())
               );

        m_Plots.push_back(A);
        A.title = "Line";
        A.yAxis = Line;
        A.colour = ImPlot::GetColormapColor(m_Plots.size());
        m_Plots.push_back(A);
    }

    PlotApp::~PlotApp() {}

    void PlotApp::update()
    {
        // Multiple step process:
        //
        // 1. Panel Setup = make sure the panel exists
        //                  and it has the right data
        // 2. Data Manipulation =   The different Calculations relating to
        //                          the analysis
        // 3. GuiRender()

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

        ShowExampleAppConsole(NULL);

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
            if(ImGui::Button("Fit to window")) ImPlot::FitNextPlotAxes();

            if(ImPlot::BeginPlot("Plot", "x", "y", {-1.0f, -1.0f}))
            {
                for(auto& plot : m_Plots)
                {
                    ImGui::PushID(&plot);
                    ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle,
                                               IMPLOT_AUTO,
                                               plot.colour,
                                               IMPLOT_AUTO,
                                               ImVec4(plot.colour.x,
                                                      plot.colour.y,
                                                      plot.colour.z,
                                                      1)
                                               );
                    ImPlot::PlotScatter(plot.title.c_str(),
                                        plot.xAxis->data(),
                                        plot.yAxis->data(),
                                        plot.xAxis->size()
                                        );
                    ImGui::PopID();
                }
                ImPlot::EndPlot();
            }
        }
        ImGui::End();
    }

    ////////////////////////////////////////////////////////////////////////////
    // PLOT PROPERTY PANEL
    ////////////////////////////////////////////////////////////////////////////

    void PlotApp::ShowPlotPanel()
    {
        if(ImGui::Begin("Plots", NULL, 0))
        {
            static size_t s_SelIdx;
            if(ImGui::BeginChild("list", ImVec2(128, 0), true))
            {
                for(size_t i = 0; i < m_Plots.size(); i++)
                {
                    ImGui::PushID(&i);
                    if(ImGui::Selectable(m_Plots[i].title.c_str(),
                                s_SelIdx == i))
                        s_SelIdx = i;
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            if(ImGui::BeginChild("properties", ImVec2(0, 0), true))
            {
                auto& plot = m_Plots[s_SelIdx];

                ImGui::ColorEdit4("Color", &plot.colour.x);

                ImGui::Separator();

                static ImGuiTableFlags flags = ImGuiTableFlags_Borders
                    | ImGuiTableFlags_RowBg;
                if(ImGui::BeginTable("values", 2, flags))
                {
                    ImGui::TableSetupColumn("X Values");
                    ImGui::TableSetupColumn("Y Values");
                    ImGui::TableHeadersRow();
                    for(size_t row_n = 0;
                            row_n < plot.xAxis->size();
                            row_n++)
                    {
                        ImGui::PushID(&plot.xAxis[row_n]);
                        ImGui::TableNextColumn();
                        ImGui::Text("%.3f", plot.xAxis[row_n]);
                        ImGui::PopID();
                        ImGui::PushID(&plot.yAxis[row_n]);
                        ImGui::TableNextColumn();
                        ImGui::Text("%.3f", plot.yAxis[row_n]);
                        ImGui::PopID();
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
                    ImGui::PushID(axis.get());
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", axis.label.c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text("[%.3f, %.3f, %.3f ...",
                            axis[0],
                            axis[1],
                            axis[2]
                            );
                    ImGui::SameLine();
                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
    }

}
