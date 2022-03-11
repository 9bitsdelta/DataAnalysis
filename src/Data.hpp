#pragma once

#include "imgui.h"
#include <initializer_list>

// THE QUESTION:
//      What is more important? The Axes or the plots?
//      - If I do scripting, I'll probably have to hold the data from the guest
//      language, so the axes and the plots dissapear *almost* entirely, maybe
//      imma have to do both guest and host structures for them...
//
// THE PLAN:
//      Settle on what an axis is: if it's just a vector or more than that
//      Settle on what a plot is (two axis and a)

namespace PPP {

    template<typename T>
    class Axis
    {
    public:
        std::string label = "Axis";
    private:
        std::shared_ptr<std::vector<T>> m_pVec = std::make_shared<std::vector<T>>();

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
        Axis(std::initializer_list<T> init)
        {
            m_pVec = std::make_shared<std::vector<T>>(init);
        }

        Axis& operator=(std::initializer_list<T> init)
        {
            m_pVec = std::make_shared<std::vector<T>>(init);
        }

        ~Axis() = default;

        //Ditch this, probably useless
        const std::vector<T>* const get() const { return m_pVec.get(); }

        std::vector<T>* operator->() const { return m_pVec.get(); }
        std::vector<T>& operator*() const { return *m_pVec; }

        T& operator[](const size_t& pos) { return (*m_pVec)[pos]; }

        auto begin() { return m_pVec->begin(); }
        auto end() { return m_pVec->end(); }

        // ostream operator overload
        friend std::ostream& operator<<(std::ostream& os, const Axis& a)
        {
            const auto& vec = *a;
            os << "[ ";
            for (const auto& iterator : vec)
                os << iterator << ' ';
            os << ']';
            return os;
        }

    };

    struct Plot
    {
        std::string title = "Title";

        Axis<double> xAxis;
        Axis<double> yAxis;
    };

}
