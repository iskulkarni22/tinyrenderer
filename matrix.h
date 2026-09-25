#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include "vec.h"

class matrix2 {
    public:
        vec2 data[2];

        matrix2(const vec2& a, const vec2& b) : data{a, b} {}

        matrix2() = default;

        const vec2& operator[](const int i) const { return data[i]; }
        vec2& operator[](const int i) { return data[i]; }
};

matrix2 operator+(const matrix2& a, const matrix2& b) {
    return matrix2{a[0] + b[0], a[1] + b[1]};
}

matrix2 operator-(const matrix2& a, const matrix2& b) {
    return matrix2{a[0] - b[0], a[1] - b[1]};
}

class matrix3 {
    public:
        vec3 data[3];

        matrix3(const vec3& a, const vec3& b, const vec3& c) : data{a, b, c} {}

        matrix3() = default;

        const vec3& operator[](const int i) const { return data[i]; }
        vec3& operator[](const int i) { return data[i]; }
};

std::ostream& operator<<(std::ostream& out, const matrix3& m) {
    for (int i = 0; i < 3; i++) {
        out << "| " << m[i].x() << std::right << std::setw(5) << m[i].y() << std::right << std::setw(5) << m[i].z() << " |" << "\n";
    }
    return out;
}

matrix3 operator+(const matrix3& a, const matrix3& b) {
   return matrix3{a[0] + b[0], a[1] + b[1], a[2] + b[2]};
}

matrix3 operator-(const matrix3& a, const matrix3& b) {
    return matrix3{a[0] - b[0], a[1] - b[1], a[2] - b[2]};
}

matrix3 operator*(const matrix3& a, const double b) {
    matrix3 res;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = a[i][j] * b;
        }
    }
    return res;
}

matrix3 operator*(const double b, const matrix3& a) {
    return a * b;
}

vec3 operator*(const matrix3& a, const vec3& b) {
    vec3 res;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i] += (a[i][j] * b[j]);
        }
    }
    return res;
}

double det(const matrix3& m) {
    double a = m[0].x();
    double b = m[0].y();
    double c = m[0].z();
    double d = m[1].x();
    double e = m[1].y();
    double f = m[1].z();
    double g = m[2].x();
    double h = m[2].y();
    double i = m[2].z();

    return a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g);
}

matrix3 transpose(const matrix3& m) {
    matrix3 res;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = m[j][i];            
        }
    }
    return res;
}

matrix3 inverse(const matrix3& m) {
    double a = m[0].x();
    double b = m[0].y();
    double c = m[0].z();
    double d = m[1].x();
    double e = m[1].y();
    double f = m[1].z();
    double g = m[2].x();
    double h = m[2].y();
    double i = m[2].z();

    double inv_det = 1 / det(m);
    matrix3 inv{{e*i-f*h, -(b*i-c*h), (b*f-c*e)},
                {-(d*i-f*g), a*i-c*g, -(a*f-c*d)},
                {d*h-e*g, -(a*h-b*g), (a*e-b*d)}};
    return inv_det * inv;
}

matrix3 inverse_transpose(const matrix3& m) {
    return inverse(transpose(m));
}

class matrix4 {
    private:
        vec4 data[4];
    public:
        matrix4(const vec4& a, const vec4& b, const vec4& c, const vec4& d) : data{a, b, c, d} {}
        
        matrix4() = default;

        const vec4& operator[](const int i) const { return data[i]; }
        vec4& operator[](const int i) { return data[i]; }
};

std::ostream& operator<<(std::ostream& out, const matrix4& m) {
    for (int i = 0; i < 4; i++) {
        out << "| " << m[i].x() << std::right << std::setw(5) 
                    << m[i].y() << std::right << std::setw(5) 
                    << m[i].z() << std::setw(5) << std::right 
                    << m[i].w() << " |" << "\n";
    }
    return out;
}

matrix4 operator+(const matrix4& a, const matrix4& b) {
   return matrix4{a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3]};
}

matrix4 operator-(const matrix4& a, const matrix4& b) {
    return matrix4{a[0] - b[0], a[1] - b[1], a[2] - b[2], a[3] - b[3]};
}

matrix4 operator*(const matrix4& a, const matrix4& b) {
    matrix4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += (a[i][k] * b[k][j]);
            }
            res[i][j] = sum;
        }
    }
    return res;
}

vec4 operator*(const matrix4& a, const vec4& b) {
    vec4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i] += (a[i][j] * b[j]);
        }
    }
    return res;
}

matrix4 transpose(const matrix4& m) {
    matrix4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = m[j][i];
        }
    }
    return res;
}


#endif