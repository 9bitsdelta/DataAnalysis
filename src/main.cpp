#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>

class TestApp : public mahi::gui::Application
{
public:
    TestApp() 
        :Application(1280, 720, "Test")
    {
        m_Plot = {
            1.0f, 0.0f, -1.0f
        };

    }

    void update() override
    {
        ImGui::StyleColorsDark();

        ImGuiBarDock();

        ImGui::Begin("Example", NULL);

        if (ImPlot::BeginPlot("Plot"))
        {
            ImPlot::PlotLine<float>("label", m_Plot.data(), m_Plot.data(), m_Plot.size());
            ImPlot::EndPlot();
        }

        ImGui::End();
    }

    void ImGuiBarDock()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::Text("Foo");
                ImGui::Separator();
                if (ImGui::MenuItem("Quit")) { Application::quit(); }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    }

private:
    std::vector<float> m_Plot;

};

int main() {
    TestApp app;
    app.run();
    return 0;
}