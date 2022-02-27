#pragma once

#include <initializer_list>
#include <memory>
#include <vector>
#include <iostream>

namespace PPP {

    // Wrapper class for std::shared_ptr of std::vector
    // - Meant to allow the making of plots which share the values of axes
    template<typename T>
    class Axis
    {
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

        const std::vector<T>* get() const { return m_pVec.get(); }

        std::vector<T>* operator->() const { return m_pVec.get(); }
        std::vector<T>& operator*() const { return *m_pVec; }

        T& operator[](size_t pos) { return (*m_pVec)[pos]; }

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

}
