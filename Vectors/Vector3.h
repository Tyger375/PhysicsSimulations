#ifndef PHYSICSSIMULATIONS_VECTOR3_H
#define PHYSICSSIMULATIONS_VECTOR3_H

class Vector3 {
public:
    double x, y, z;

    Vector3() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vector3(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    [[nodiscard]] inline double dot(const Vector3& other) const { return this->x * other.x + this->y * other.y + this->z * other.z; }
    [[nodiscard]] inline Vector3 cross(const Vector3& other) const {
        return {
                (this->y * other.z) - (this->z * other.y),
                (this->z * other.x) - (this->x * other.z),
            (this->x * other.y) - (this->y * other.x)
        };
    }

    Vector3 &operator+=(const Vector3 &other) {
        this->x += other.x;
        this->y += other.y;
        this->z += other.z;
        return *this;
    }

    friend Vector3 operator+(Vector3 first, const Vector3 &second) {
        first += second;
        return first;
    }

    Vector3 &operator*=(const double &other)
    {
        this->x *= other;
        this->y *= other;
        this->z *= other;
        return *this;
    }

    friend Vector3 operator*(Vector3 first, const double& second)
    {
        first *= second;
        return first;
    }
};

#endif //PHYSICSSIMULATIONS_VECTOR3_H
