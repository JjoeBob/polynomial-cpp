//
// Created by Cowabunga on 09.10.2026.
//

#ifndef POLYNOMIAL_OBSERVABLE_H
#define POLYNOMIAL_OBSERVABLE_H
#include <functional>
#include <vector>

template <typename T>
class Observable
{
public:
    using Observer = std::function<void(const T&)>;

    void setValue(const T& newValue)
    {
        m_value = newValue;
        notify();
    };

    const T& getValue() const
    {
        return m_value;
    };

    void observe(const Observer& observer)
    {
        m_observers.push_back(observer);
        observer(m_value);
    }

private:
    T m_value{};
    std::vector<Observer> m_observers;

    void notify()
    {
        for (const auto& observer : m_observers)
        {
            observer(m_value);
        }
    };
};


#endif //POLYNOMIAL_OBSERVABLE_H
