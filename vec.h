#ifndef VEC_H
#define VEC_H

#include <iostream>
#include <math.h>


template<int dims> struct vec {
    double data[dims] = {0};
    double operator[](const int i) const { return data[i]; }
    double& operator[](const int i) { return data[i]; }
};

template<> struct vec<2> {
    double x = 0, y = 0;
    double operator[](const int i) const { return i ? y : x; }
    double& operator[](const int i) { return i ? y : x; }

    inline double dot(const vec<2> other) const {
        return x*other.x + y*other.y;
    }
};

template<> struct vec<3> {
    double x = 0, y = 0, z = 0;
    double operator[](const int i) const { return i ? (i==2 ? z : y) : x; }
    double& operator[](const int i) { return i ? (i==2 ? z : y) : x; }

    inline double dot(const vec<3> other) const {
        return x*other.x + y*other.y + z*other.z;
    }

    inline vec<3> cross(const vec<3>& other) const {
        return {y*other.z - z*other.y,
                z*other.x - x*other.z,
                x*other.y - y*other.x};
    }
};

template<> struct vec<4> {
    double x = 0, y = 0, z = 0, w = 0;
    double operator[](const int i) const { return i<2 ? (i ? y : x) : (i==2 ? z : w); }
    double& operator[](const int i) { return i<2 ? (i ? y : x) : (i==2 ? z : w); }
    vec<2> xy() const { return {x, y}; }
    vec<3> xyz() const { return {x, y, z}; }

    inline double dot(const vec<4> other) const {
        return x*other.x + y*other.y + z*other.z + w*other.w;
    }
};

using vec2 = vec<2>;
using vec3 = vec<3>;
using vec4 = vec<4>;
using vertex = vec3;

template<int n>
std::ostream& operator<<(std::ostream& out, const vec<n> & v) {
    out << "<";
    for (int i = 0; i < n; i++) out << v[i] << " ";
    out << ">";
}

template<int n>
vec<n> operator+(const vec<n>& a, const vec<n>& b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] + b[i];
    return res;
}

template<int n>
vec<n> operator-(const vec<n>& a, const vec<n>& b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] - b[i];
    return res;
}

template<int n>
vec<n> operator*(const vec<n>& a, const vec<n>& b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] * b[i];
    return res;
}

template<int n>
vec<n> operator/(const vec<n>& a, const vec<n>& b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] / b[i];
    return res;
}

template<int n>
vec<n> operator*(const vec<n>& a, const double b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] * b;
    return res;
}

template<int n>
vec<n> operator*(const double b, const vec<n>& a) {
    return a * b;
}

template<int n>
vec<n> operator/(const vec<n>& a, const double b) {
    vec<n> res;
    for (int i = 0; i < n; i++) res[i] = a[i] / b;
    return res;
}

template<int n>
double norm(const vec<n>& a) {
    double res = 0.;
    for (int i = 0; i < n; i++) res += sqrt(a[i]*a[i]);
    return res;
}

template<int n>
vec<n> normalized(const vec<n>& a) {
    return a / norm(a);
}

template<int n>
vec<n+1> add_dim(const vec<n>& a) {
    return {a.x, a.y, a.z, 0};
}

#endif