#pragma once
#include "vector2.hpp"

class Vector3 : public Vector2 {
public:
    float z;

    explicit Vector3(const float x = 0.0f, const float y = 0.0f, const float z = 0.0f)
        : Vector2(x, y), z(z) {}

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    void normalize() {
        if (float len = length(); len != 0.0f) {
            x /= len;
            y /= len;
            z /= len;
        }
    }

    void print() const {
        std::cout << "Vector3(" << x << ", " << y << ", " << z << ")\n";
    }
};
