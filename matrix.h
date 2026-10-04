#ifndef MATRIX_H
#define MATRIX_H

#include <iomanip>
#include "vec.h"

template<int rows, int cols> struct matrix {
    vec<cols> data[rows] = {{}};
    const vec<cols>& operator[](const int i) const { return data[i]; }
    vec<cols>& operator[](const int i) { return data[i]; }
};

using matrix2 = matrix<2,2>;
using matrix3 = matrix<3,3>;
using matrix4 = matrix<4,4>;

template<int rows, int cols>
std::ostream& operator<<(std::ostream& out, const matrix<rows,cols>& m) {
    for (int i = 0; i < rows; i++) out << m[i] << std::endl;
    return out;
}

template<int rows, int cols>
matrix<rows,cols> operator+(const matrix<rows,cols>& a, const matrix<rows,cols>& b) {
    matrix<rows,cols> res;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            res[i][j] = a[i][j] + b[i][j];
    return res;
}

template<int rows, int cols>
matrix<rows,cols> operator-(const matrix<rows,cols>& a, const matrix<rows,cols>& b) {
    matrix<rows,cols> res;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            res[i][j] = a[i][j] - b[i][j];
    return res;
}

template<int rows, int cols>
matrix<rows,cols> operator*(const matrix<rows,cols>& a, const double b) {
    matrix<rows,cols> res;
    for (int i = 0; i < rows; i++)
        res[i] = a[i] * b;
    return res;
}

template<int rows, int cols>
matrix<rows,cols> operator*(const double b, const matrix<rows,cols>& a) {
    return a * b;
}

template<int rows, int cols>
vec<cols> operator*(const matrix<rows,cols>& a, const vec<cols> b) {
    vec<cols> res;
    for (int i = 0; i < rows; i++)
        res[i] = a[i].dot(b);
    return res;
}

template<int r1, int c1, int c2>
matrix<r1,c2> operator*(const matrix<r1,c1>& a, const matrix<c1,c2>& b) {
    matrix<r1,c2> res;
    for (int i = 0; i < r1; i++)
        for (int j = 0; j < c2; j++)
            for (int k = 0; k < c1; k++) res[i][j] += a[i][k] * b[k][j];
    return res;
}

template<int rows, int cols>
matrix<rows,cols> operator/(const matrix<rows,cols>& a, const double b) {
    matrix<rows,cols> res;
    for (int i = 0; i < rows; i++)
        res[i] = a[i] / b;
    return res;
}

template<int rows, int cols>
matrix<rows, cols> transpose(const matrix<rows, cols>& m) {
    matrix<cols, rows> res;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            res[i][j] = m[j][i];
        }
    }
    return res;
}

inline double det(const matrix2& m) {
    double a = m[0].x;
    double b = m[0].y;
    double c = m[1].x;
    double d = m[1].y;

    return a*d - b*c;
}

inline matrix2 inverse(const matrix2& m) {
    double a = m[0].x;
    double b = m[0].y;
    double c = m[1].x;
    double d = m[1].y;

    double inv_det = 1 / det(m);
    matrix2 inv = {{{d, -b}, 
                    {-c, a}}};
    return inv_det * inv;
}

inline double det(const matrix3& m) {
    double a = m[0].x;
    double b = m[0].y;
    double c = m[0].z;
    double d = m[1].x;
    double e = m[1].y;
    double f = m[1].z;
    double g = m[2].x;
    double h = m[2].y;
    double i = m[2].z;

    return a*(e*i-f*h) - b*(d*i-f*g) + c*(d*h-e*g);
}

inline matrix3 inverse(const matrix3& m) {
    double a = m[0].x;
    double b = m[0].y;
    double c = m[0].z;
    double d = m[1].x;
    double e = m[1].y;
    double f = m[1].z;
    double g = m[2].x;
    double h = m[2].y;
    double i = m[2].z;

    double inv_det = 1 / det(m);
    matrix3 inv{{{e*i-f*h, -(b*i-c*h), (b*f-c*e)},
                {-(d*i-f*g), a*i-c*g, -(a*f-c*d)},
                {d*h-e*g, -(a*h-b*g), (a*e-b*d)}}};
    return inv_det * inv;
}

inline matrix3 inverse_transpose(const matrix3& m) {
    return inverse(transpose(m));
}

inline matrix3 to_matrix3(const matrix4& m) {
    matrix3 res;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) res[i][j] = m[i][j];
    return res;
}


#endif