#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <math.h>


class vec2 {
    private:
        double data[2] = {0., 0.};
    public:
        vec2(double x, double y) : data{x, y} {}

        vec2() = default;

        double operator[](const int i) const { return data[i]; }
        double& operator[](const int i) { return data[i]; }
        
        double x() const { return data[0]; }
        double y() const { return data[1]; }

        const double length() const {
            return sqrt(x()*x() + y()*y());
        }

        const double dot(const vec2& other) const {
            return x() * other.x() + y() * other.y();
        }
};

inline std::ostream& operator<<(std::ostream& out, const vec2& a) {
    out << '<' << a.x() << ", " << a.y() << '>';
    return out;
}

inline vec2 operator+(const vec2& a, const vec2& b) {
    return vec2(a.x() + b.x(), a.y() + b.y());
}

inline vec2 operator-(const vec2& a, const vec2& b) {
    return vec2(a.x() - b.x(), a.y() - b.y());
}

inline vec2 operator*(const vec2& a, const vec2& b) {
    return vec2(a.x() * b.x(), a.y() * b.y());
}

inline vec2 operator/(const vec2& a, const vec2& b) {
    return vec2(a.x() / b.x(), a.y() / b.y());
}


class vec3 {
    private:
        double data[3] = {0., 0., 0.};
    public:        
        vec3(double x, double y, double z) : data{x,y,z} {}
        vec3(double x, double y) : vec3(x, y, 0) {}

        vec3() = default;

        double operator[](const int i) const { return data[i]; }
        double& operator[](const int i) { return data[i]; }

        double x() const { return data[0]; }
        double y() const { return data[1]; }
        double z() const { return data[2]; }

        const double length() const {
            return sqrt(x()*x() + y()*y() + z()*z());
        }

        const double dot(const vec3& other) const {
            return x() * other.x() + y() * other.y() + z() * other.z();
        }

        const vec3 cross(const vec3& other) const {
            return vec3(y()*other.z() - z()*other.y(),
                        z()*other.x() - x()*other.z(),
                        x()*other.y() - y()*other.x());
        }
};

inline std::ostream& operator<<(std::ostream& out, const vec3& a) {
    out << '<' << a.x() << ", " << a.y() << ", " << a.z() << '>';
    return out;
}

inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline vec3 operator*(const vec3& a, const vec3& b) {
    return vec3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z());
}

inline vec3 operator*(const vec3& a, double b) {
    return vec3(a.x() * b, a.y() * b, a.z() * b);
}

inline vec3 operator/(const vec3& a, const vec3& b) {
    return vec3(a.x() / b.x(), a.y() / b.y(), a.z() / b.z());
}

inline vec3 operator/(const vec3& a, double b) {
    return a * (1/b);
}

inline double norm(const vec3& a) {
    vec3 squared = a*a;
    return sqrt(squared.x()) + sqrt(squared.y()) + sqrt(squared.z());
}

inline vec3 normalized(const vec3& a) {
    return a / norm(a);
}

using vertex = vec3;


class vec4 {
    private:
        double data[4] = {0., 0., 0., 0.};
    public:
        vec4(double x, double y, double z, double w) : data{x,y,z,w} {}

        vec4() = default;

        double operator[](const int i) const { return data[i]; }
        double& operator[](const int i) { return data[i]; }

        double x() const { return data[0]; }
        double y() const { return data[1]; }
        double z() const { return data[2]; }
        double w() const { return data[3]; }

        const vec2 xy() const { return vec2(x(), y()); }
        const vec3 xyz() const { return vec3(x(), y(), z()); }

        const double length() const {
            return sqrt(x()*x() + y()*y() + z()*z());
        }

        const double dot(const vec4& other) const {
            return x() * other.x() + y() * other.y() + z() * other.z() + w() * other.w();
        }
};

inline std::ostream& operator<<(std::ostream& out, const vec4& a) {
    out << '<' << a.x() << ", " << a.y() << ", " << a.z() << ", " << a.w() << '>';
    return out;
}

inline vec4 operator+(const vec4& a, const vec4& b) {
    return vec4(a.x() + b.x(), a.y() + b.y(), a.z() + b.z(), a.w() + b.w());
}

inline vec4 operator-(const vec4& a, const vec4& b) {
    return vec4(a.x() - b.x(), a.y() - b.y(), a.z() - b.z(), a.w() - b.w());
}

inline vec4 operator*(const vec4& a, const vec4& b) {
    return vec4(a.x() * b.x(), a.y() * b.y(), a.z() * b.z(), a.w() * b.w());
}

inline vec4 operator*(const vec4& a, double b) {
    return vec4(a.x() * b, a.y() * b, a.z() * b, a.w() * b);
}

inline vec4 operator/(const vec4& a, const vec4& b) {
    return vec4(a.x() / b.x(), a.y() / b.y(), a.z() / b.z(), a.w() / b.w());
}

inline vec4 operator/(const vec4& a, double b) {
    return a * (1/b);
}

#endif