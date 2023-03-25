#include "optional.h"

template<typename T> Option<T>::Option() : is_set(false) {}

template<typename T> Option<T>::Option(T value) : value(value), is_set(true) {}

template<typename T> Option<T>::Option(const Option<T>& other) : value(other.value), is_set(other.is_set) {}

template<typename T> Option<T>& Option<T>::operator=(const Option<T>& other) {
    value = other.value;
    is_set = other.is_set;
    return *this;
}

template<typename T> bool Option<T>::operator==(const Option<T>& other) {
    return value == other.value && is_set == other.is_set;
}

template<typename T> bool Option<T>::isSet() {
    return is_set;
}

template<typename T> T Option<T>::get() const {
    return value;
}
template<typename T> void Option<T>::set(T value) {
    this->value = value;
    is_set = true;
}

template<typename T> void Option<T>::unset() {
    is_set = false;
}
