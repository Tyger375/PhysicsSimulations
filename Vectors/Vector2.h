#ifndef PHYSICSSIMULATIONS_VECTOR2_H
#define PHYSICSSIMULATIONS_VECTOR2_H

#include <string>
#include <cmath>
#include <ostream>
#include <SFML/System/Vector2.hpp>
#include "Vector3.h"

/*
 * Extension of sf::Vector2f class
 * Added basic vector arithmetic
 * */

class Vector2 {
public:
    double x, y;

    Vector2() {
        this->x = 0;
        this->y = 0;
    }

    Vector2(double x, double y) {
        this->x = x;
        this->y = y;
    }

    explicit Vector2(sf::Vector2f v) {
        this->x = v.x;
        this->y = v.y;
    }

    [[nodiscard]] std::string to_string() const {
        std::string str;
        str += "(";
        str += std::to_string(x);
        str += ", ";
        str += std::to_string(y);
        str += ")";
        return str;
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

    Vector2 &operator*=(const double &other)
    {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    friend Vector2 operator*(Vector2 first, const double& second)
    {
        first *= second;
        return first;
    }

    friend Vector2 operator*(const double& first, Vector2 second)
    {
        second *= first;
        return second;
    }

    Vector2 &operator/=(const double &other)
    {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    friend Vector2 operator/(Vector2 first, const double& second)
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
        return {(float)this->x, (float)this->y};
    }

    friend bool operator==(const Vector2& first, const Vector2& other)
    {
        const float TOLERANCE = 0.0001;
        auto a = std::abs(first.x - other.x) < TOLERANCE;
        auto b = std::abs(first.y - other.y) < TOLERANCE;
        return a && b;
    }
    #pragma endregion

    [[nodiscard]] inline double dot(const Vector2& other) const { return this->x * other.x + this->y * other.y; }
    [[nodiscard]] inline Vector3 cross(const Vector2& other) const { return {0,0,(this->x * other.y) - (this->y * other.x)}; }

    [[nodiscard]] inline double magnitude() const { return std::sqrt(dot(*this)); }

    [[nodiscard]] inline Vector2 normalize() const { return *this / magnitude(); }

    [[nodiscard]] inline Vector2 orthogonal() const { return {-this->y, this->x}; }
};

#endif //PHYSICSSIMULATIONS_VECTOR2_H
