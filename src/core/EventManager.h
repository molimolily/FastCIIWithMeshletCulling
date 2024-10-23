#pragma once

#include<vector>
#include<functional>

template<typename... Args>
class EventManager {
public:
    using ObserverFunc = std::function<void(Args...)>;
    std::vector<ObserverFunc> observers;

    void Subscribe(const ObserverFunc& observer) {
        observers.push_back(observer);
    }

    void Notify(Args... args) {
        for (auto& observer : observers) {
            observer(args...);
        }
    }
};
