#pragma once

#include "imgui.h"
#include <initializer_list>

namespace PPP {

    class Axis
    {
    public:
        std::string label = "Axis";
    private:
        std::shared_ptr<std::vector<float>> m_pVec =
            std::make_shared<std::vector<float>>();

    public:
        Axis() = default;

        Axis(const Axis& other) = default;

        Axis& operator=(const Axis& other) = default;

        Axis(Axis&& other)
        {
            *(m_pVec.get()) = std::move(*(other.m_pVec.get()));
        }

        Axis& operator=(Axis&& other)
        {
            *(m_pVec.get()) = std::move(*(other.m_pVec.get()));
            return *this;
        }

        // Inits object from a list
        Axis(std::initializer_list<float> init)
        {
            m_pVec = std::make_shared<std::vector<float>>(init);
        }

        Axis& operator=(std::initializer_list<float> init)
        {
            m_pVec = std::make_shared<std::vector<float>>(init);
            return *this;
        }

        ~Axis() = default;

        //Ditch this, probably useless
        const std::vector<float>* const get() const { return m_pVec.get(); }

        std::vector<float>* operator->() const { return m_pVec.get(); }
        std::vector<float>& operator*() const { return *m_pVec; }

        float& operator[](const size_t& pos) { return (*m_pVec)[pos]; }

        // ostream operator overload
        friend std::ostream& operator<<(std::ostream& os, const Axis& a)
        {
            const auto& vec = *a;
            os << "[ ";
            for(const auto& iterator : vec)
                os << iterator << ' ';
            os << ']';
            return os;
        }

        friend bool operator==(const Axis& lhs, const Axis& rhs)
        {
            return (lhs.get() == rhs.get());
        }

    };

    class Plot
    {
    public:
        std::string title = "Title";

        ImVec4 colour;

        Axis xAxis;
        Axis yAxis;

    public:
        Plot() = default;

        Plot(const std::string& title,
             const Axis& x,
             const Axis& y,
             const ImVec4& colour)
        {
            this->title = title;
            this->xAxis = x;
            this->yAxis = y;
            this->colour = colour;
        }

        friend bool operator==(const Plot& lhs, const Plot& rhs)
        {
            return (lhs.title == rhs.title)
                 &&(lhs.xAxis == rhs.xAxis)
                 &&(lhs.yAxis == rhs.yAxis);
        }

    };

}
