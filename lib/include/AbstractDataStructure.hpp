#pragma once
#include "IDataStructure.hpp"
#include "IObservable.hpp"
#include <vector>

namespace ds {
    class AbstractDataStructure : public IDataStructure, public IObservable {
    public:
        virtual ~AbstractDataStructure() = default;
        void notify() override;
    protected:
        std::vector<int> m_elements;
    };
}