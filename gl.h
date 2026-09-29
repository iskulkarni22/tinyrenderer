#ifndef GL_H
#define GL_H

#include "tgaimage.h"
#include "matrix.h"


void lookat(const vec3 eye, const vec3 center, const vec3 up);
void viewport(const int x, const int y, const int w, const int h);
void perspective(const double f);
void init_zbuffer(const int width, const int height);

struct IShader {
    virtual std::pair<bool, TGAColor> frag(const vec3 bar) const = 0;

    static TGAColor sample2D(const TGAImage& img, const vec2& uv) {
        return img.get(uv[0] * img.width(), uv[1] * img.height());
    }
};

typedef vec4 Triangle[3];
vec3 surface_norm(const vec3 tri[3]);
void rasterize(TGAImage& framebuffer, const Triangle& clip, const IShader& shader);


#endif