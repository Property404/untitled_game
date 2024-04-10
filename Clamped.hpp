#pragma once
#include <cassert>
#include <algorithm>

template<typename T>
class Clamped {
    T _val{};
    T _min{};
    T _max{};

    public:
    Clamped(T val, T min, T max):
        _val(val),
        _min(min),
        _max(max)
    {
        assert(val >= min);
        assert(val <= max);
    }

    Clamped<T> operator +(T val) const {
        return this->_val + val;
    }
};
