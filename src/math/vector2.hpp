#pragma once
#include <iostream>
#include <cmath>

class Vector2 {
public:
    float x, y;

    explicit Vector2(const float x = 0.0f, const float y = 0.0f) : x(x), y(y) {}

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    void normalize() {
        if (float len = length(); len != 0.0f) {
            x /= len;
            y /= len;
        }
    }

    void print() const {
        std::cout << "Vector2(" << x << ", " << y << ")\n";
    }
};
