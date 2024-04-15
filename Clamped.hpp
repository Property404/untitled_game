#pragma once
#include <cassert>
#include <algorithm>
#include <ostream>

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

    T inner() const {
        return this->_val;
    }

    bool operator ==(T val) const {
        return _val == val;
    }

    bool operator >=(T val) const {
        return _val >= val;
    }

    bool operator >(T val) const {
        return _val > val;
    }

    bool operator <=(T val) const {
        return _val <= val;
    }

    bool operator <(T val) const {
        return _val < val;
    }

    friend std::ostream& operator<< (std::ostream& stream, const Clamped<T>& that) {
        return (stream << that.inner());
    }

    Clamped<T> operator -(T val) const {
        if (_min >= 0 && val > _val) {
            return Clamped(0, _min, _max);
        }
        return Clamped(std::clamp(_val - val, _min, _max), _min, _max);
    }

    Clamped<T> operator +(T val) const {
        return Clamped(std::clamp(_val + val, _min, _max), _min, _max);
    }

    Clamped<T> operator *(T val) const {
        return Clamped(std::clamp(_val * val, _min, _max), _min, _max);
    }

    Clamped<T> operator /(T val) const {
        return Clamped(std::clamp(_val / val, _min, _max), _min, _max);
    }

    Clamped<T> operator %(T val) const {
        return Clamped(std::clamp(_val % val, _min, _max), _min, _max);
    }

    Clamped<T> operator ^(T val) const {
        return Clamped(std::clamp(_val ^ val, _min, _max), _min, _max);
    }

    Clamped<T> operator &(T val) const {
        return Clamped(std::clamp(_val & val, _min, _max), _min, _max);
    }

    Clamped<T> operator |(T val) const {
        return Clamped(std::clamp(_val | val, _min, _max), _min, _max);
    }

    Clamped<T> operator =(T val){
        return (*this = Clamped(std::clamp(val,_min,_max), _min, _max));
    }

    Clamped<T> operator -=(T val){
        return (*this = *this - val);
    }

    Clamped<T> operator +=(T val){
        return (*this = *this + val);
    }

    Clamped<T> operator ++(int){
        const auto old = *this;
        *this += 1;
        return old;
    }

    Clamped<T> operator --(int){
        const auto old = *this;
        *this -= 1;
        return old;
    }

    Clamped<T> operator *=(T val){
        return (*this = *this * val);
    }

    Clamped<T> operator /=(T val){
        return (*this = *this / val);
    }

    Clamped<T> operator %=(T val){
        return (*this = *this % val);
    }
};
