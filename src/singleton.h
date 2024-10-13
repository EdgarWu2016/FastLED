#pragma once

// A templated singleton class, parameterized by the type of the singleton and an optional integer.
template<typename T, int N = 0>
class Singleton {
public:
    static T& instance() {
        static T instance;
        return instance;
    }
    static T* instancePtr() {
        return &instance();
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    Singleton() = default;
    ~Singleton() = default;
    T mInstance;
};