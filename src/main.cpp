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

        m_WindowFlags |= ImGuiWindowFlags_NoCollapse;
    }

    void update() override
    {
        //bool show = true;
        //ImGui::ShowDemoWindow(&show);

        ImGui::Begin("Example", NULL, m_WindowFlags);

        ImPlot::BeginPlot("Plot");
        ImPlot::PlotLine<float>("label", m_Plot.data(), m_Plot.data(), m_Plot.size());
        ImPlot::EndPlot();

        if (ImGui::Button("Press Me!"))
            mahi::util::print("Hello, World!");
        ImGui::End();
    }

private:
    std::vector<float> m_Plot;
    ImGuiWindowFlags m_WindowFlags = 0;
};

int main() {
    TestApp app;
    app.run();
    return 0;
}