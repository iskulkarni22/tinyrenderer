#include "tgaimage.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <limits>
#include "model.h"
#include "matrix.h"

constexpr int width  = 800;
constexpr int height = 800;

constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

matrix4 ModelView, Viewport, Perspective;

// void line(TGAImage& fb, int ax, int ay, int bx, int by, TGAColor color) {
//     bool steep = abs(by - ay) > abs(bx - ax);
//     if (steep) {
//         std::swap(ax, ay);
//         std::swap(bx, by);
//     }
//     if (ax > bx) {
//         std::swap(ax, bx);
//         std::swap(ay, by);
//     }
//     for (double x = ax; x <= bx; x++) {
//         double t = (x - ax) / static_cast<float>(bx - ax);
//         int y = ay + round(t * (by - ay));
//         if (steep) fb.set(y, x, color);
//         else fb.set(x, y, color);
//     }
// }

void rasterize(TGAImage& fb, std::vector<double>& zb, const vec4 clip[3], TGAColor color) {
    vec4 ndc[3] = {clip[0]/clip[0].w(), clip[1]/clip[1].w(), clip[2]/clip[2].w()};
    vec2 screen[3] = {(Viewport*ndc[0]).xy(), (Viewport*ndc[1]).xy(), (Viewport*ndc[2]).xy()};
    
    matrix3 ABC = {{screen[0].x(), screen[0].y(), 1.}, {screen[1].x(), screen[1].y(), 1.}, {screen[2].x(), screen[2].y(), 1.}};
    if (det(ABC) < 1) return;

    // draw bounding box around using min and max coords
    auto [min_x, max_x] = std::minmax({screen[0].x(), screen[1].x(), screen[2].x()});
    auto [min_y, max_y] = std::minmax({screen[0].y(), screen[1].y(), screen[2].y()});
    
    min_x = std::max(0., min_x);
    min_y = std::max(0., min_y);
    max_x = std::min(fb.width()-1., max_x);
    max_y = std::min(fb.height()-1., max_y);

#pragma omp parallel for
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            vec3 bc = inverse_transpose(ABC) * vec3(x, y, 1.);
            if (bc.x() < 0 || bc.y() < 0 || bc.z() < 0) continue;

            double z = bc.dot(vec3{ndc[0].z(), ndc[1].z(), ndc[2].z()});

            if (z <= zb[x+y*fb.width()]) continue;
            zb[x+y*fb.width()] = z;
            fb.set(x, y, color);
        }
    }
}

// vertex rotate(vertex v) {
//     double theta = 0;
//     matrix3 rot_mat = {{ cos(theta), 0, sin(theta) },
//                        { 0,          1, 0          },
//                        {-sin(theta), 0, cos(theta) }};

//     return rot_mat * v;
// }

// vertex persp(vertex v) {
//     double c = 3;
//     return v / (1 - v.z() / c);
// }

// vertex project(vertex v) {
//     return vertex((v.x() + 1) * width / 2, (v.y() + 1) * height / 2, v.z());
// }

void viewport(const int x, const int y, const int w, const int h) {
    Viewport = {{w/2., 0, 0, x + w/2.},
                {0, h/2., 0, y + h/2.},
                {0, 0, 1, 0},
                {0, 0, 0, 1}};
}

void perspective(const double f) {
    Perspective = {{1, 0, 0, 0},
                   {0, 1, 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, -1/f, 1}};
}

void lookat(const vec3 eye, const vec3 center, const vec3 up) {
    vec3 n = normalized(eye - center);
    vec3 l = normalized(up.cross(n));
    vec3 m = normalized(n.cross(l));
    ModelView = matrix4{
                 {l.x(), l.y(), l.z(), 0},
                 {m.x(), m.y(), m.z(), 0},
                 {n.x(), n.y(), n.z(), 0},
                 {0, 0, 0, 1}} *
                matrix4{
                 {1, 0, 0, -center.x()},
                 {0, 1, 0, -center.y()},
                 {0, 0, 1, -center.z()},
                 {0, 0, 0, 1}};
}

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    std::vector<double> zbuffer(width*height, -std::numeric_limits<double>::infinity());

    TGAImage depthImg(width, height, TGAImage::GRAYSCALE);

    std::string filepath = argv[1];

    model mesh;
    mesh.read_obj_file(filepath);

    const vec3 eye(-1,0,2);
    const vec3 center(0,0,0);
    const vec3 up(0,1,0);

    lookat(eye, center, up);
    perspective(norm(eye - center));
    viewport(width/16, height/16, width*7/6, height*7/8);

    // draw triangles
    // iterate over faces
    for (int i = 0; i < mesh.faces.size(); i++) {
        face f = mesh.faces[i];

        vec4 clip[3];
        for (int i = 0; i < 3; i++) {
            vertex v = mesh.vertices[f.vertex_idx[i]];
            clip[i] = Perspective * ModelView * vec4(v.x(), v.y(), v.z(), 1);
        }

        TGAColor rand_color;
        for (int c = 0; c < 3; c++) rand_color[c] = rand() % 255;
        rasterize(framebuffer, zbuffer, clip, rand_color);
    }

    // convert zbuffer data to depth image
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char z = (zbuffer[x+y*framebuffer.width()]+1) * 255/2;
            depthImg.set(x, y, {z});
        }
    }

    framebuffer.write_tga_file("framebuffer.tga");
    depthImg.write_tga_file("zbuffer.tga");

    return 0;
}
