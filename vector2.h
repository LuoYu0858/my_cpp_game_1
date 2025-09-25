#ifndef PLANTSVSPLANTS_VECTOR2_H
#define PLANTSVSPLANTS_VECTOR2_H

// 二维向量类
class Vector2 {
public:
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
    ~Vector2() = default;

    Vector2 operator+(const Vector2& v) const {
        return {x + v.x, y + v.y};
    }

    void operator+=(const Vector2& v) {
        x += v.x, y += v.y;
    }

    Vector2 operator-(const Vector2& v) const {
        return {x - v.x, y - v.y};
    }

    void operator-=(const Vector2& v) {
        x -= v.x, y -= v.y;
    }

    float operator*(const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    Vector2 operator*(const float v) const {
        return {x * v, y * v};
    }

    void operator*=(const float v) {
        x *= v, y *= v;
    }

    // 向量长度
    [[nodiscard]] float length() const {
        return std::sqrt(x * x + y * y);
    }

    // 获取标准化向量
    [[nodiscard]] Vector2 normalize() const {
        float l = length();
        return l == 0 ? Vector2(0, 0) : Vector2(x / l, y / l);
    }

public:
    float x = 0.0f;
    float y = 0.0f;
};

#endif //PLANTSVSPLANTS_VECTOR2_H
