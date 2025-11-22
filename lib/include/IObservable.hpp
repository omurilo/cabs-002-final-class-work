#pragma once

namespace ds {
    class IObservable {
    public:
        virtual ~IObservable() = default;
        virtual void notify() = 0;
    };
}