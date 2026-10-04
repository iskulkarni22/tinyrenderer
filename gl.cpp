#include "gl.h"


matrix4 ModelView, Viewport, Perspective;
std::vector<double> zbuffer;

void viewport(const int x, const int y, const int w, const int h) {
    Viewport = {{{w/2., 0, 0, x + w/2.},
                 {0, h/2., 0, y + h/2.},
                 {0, 0, 1, 0},
                 {0, 0, 0, 1}}};
}

void perspective(const double f) {
    Perspective = {{{1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, -1/f, 1}}};
}

void lookat(const vec3 eye, const vec3 center, const vec3 up) {
    vec3 n = normalized(eye - center);
    vec3 l = normalized(up.cross(n));
    vec3 m = normalized(n.cross(l));
    ModelView = matrix4{
                 {{l.x, l.y, l.z, 0},
                  {m.x, m.y, m.z, 0},
                  {n.x, n.y, n.z, 0},
                  {0, 0, 0, 1}}} *
                matrix4{
                 {{1, 0, 0, -center.x},
                  {0, 1, 0, -center.y},
                  {0, 0, 1, -center.z},
                  {0, 0, 0, 1}}};
}

void init_zbuffer(const int width, const int height) {
    zbuffer = std::vector<double>(width*height, -std::numeric_limits<double>::infinity());
}

void rasterize(TGAImage& fb, const Triangle& clip, const IShader& shader) {
    vec4 ndc[3] = {clip[0]/clip[0].w, clip[1]/clip[1].w, clip[2]/clip[2].w};
    vec2 screen[3] = {(Viewport*ndc[0]).xy(), (Viewport*ndc[1]).xy(), (Viewport*ndc[2]).xy()};
    
    matrix3 ABC = {{{screen[0].x, screen[0].y, 1.}, {screen[1].x, screen[1].y, 1.}, {screen[2].x, screen[2].y, 1.}}};
    if (det(ABC) < 1) return;

    // draw bounding box around using min and max coords
    auto [min_x, max_x] = std::minmax({screen[0].x, screen[1].x, screen[2].x});
    auto [min_y, max_y] = std::minmax({screen[0].y, screen[1].y, screen[2].y});
    
    min_x = std::max(0., min_x);
    min_y = std::max(0., min_y);
    max_x = std::min(fb.width()-1., max_x);
    max_y = std::min(fb.height()-1., max_y);

#pragma omp parallel for
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            vec3 bc = inverse_transpose(ABC) * vec3{static_cast<double>(x), static_cast<double>(y), 1.};
            if (bc.x < 0 || bc.y < 0 || bc.z < 0) continue;
            double z = bc.dot(vec3{ndc[0].z, ndc[1].z, ndc[2].z});
            if (z <= zbuffer[x+y*fb.width()]) continue;

            auto [discard, color] = shader.frag(bc);
            if (discard) continue;
            
            zbuffer[x+y*fb.width()] = z;
            fb.set(x, y, color);
        }
    }
}