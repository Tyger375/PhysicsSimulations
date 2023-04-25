#ifndef PHYSICSSIMULATIONS_VECTOR2_H
#define PHYSICSSIMULATIONS_VECTOR2_H

#include <cmath>
#include <ostream>
#include <SFML/System/Vector2.hpp>

/*
 * Extension of sf::Vector2f class
 * Added basic vector arithmetic
 * */
class Vector2 {
public:
    float x, y;

    Vector2() {
        this->x = 0;
        this->y = 0;
    }

    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2(sf::Vector2f v) {
        this->x = v.x;
        this->y = v.y;
    }

    #pragma region Operators
    Vector2 &operator+=(const Vector2 &other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    friend Vector2 operator+(Vector2 first, const Vector2 &second) {
        first += second;
        return first;
    }

    Vector2 &operator-=(const Vector2 &other) {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    friend Vector2 operator-(Vector2 first, const Vector2 &second) {
        first -= second;
        return first;
    }

    Vector2 &operator*=(const float &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    friend Vector2 operator*(Vector2 first, const float& second)
    {
        first *= second;
        return first;
    }

    friend Vector2 operator*(const float& first, Vector2 second)
    {
        second *= first;
        return second;
    }

    Vector2 &operator/=(const float &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    friend Vector2 operator/(Vector2 first, const float& second)
    {
        first /= second;
        return first;
    }

    friend std::ostream &operator<<(std::ostream& os, const Vector2& v)
    {
        return os << '(' << v.x << ", " << v.y << ')';
    }

    explicit operator sf::Vector2f() const
    {
        return {this->x, this->y};
    }

    friend bool operator==(const Vector2& first, const Vector2& other)
    {
        const float TOLERANCE = 0.1;
        auto a = std::abs(first.x - other.x) < TOLERANCE;
        auto b = std::abs(first.y - other.y) < TOLERANCE;
        return a && b;
    }
    #pragma endregion

    [[nodiscard]] inline float dot(const Vector2& other) const { return this->x * other.x + this->y * other.y; }

    [[nodiscard]] inline float magnitude() const { return std::sqrt(dot(*this)); }

    [[nodiscard]] inline Vector2 normalize() const { return *this / magnitude(); }
};


#endif //PHYSICSSIMULATIONS_VECTOR2_H
