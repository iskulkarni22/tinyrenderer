#include "tgaimage.h"
#include <utility>
#include <iostream>
#include <fstream>
#include <limits>
#include "model.h"
#include "gl.h" 


constexpr TGAColor white   = {255, 255, 255, 255}; // BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

extern matrix4 ModelView, Perspective;
extern std::vector<double> zbuffer;


struct RandomShader : IShader {
    const model &mesh;
    TGAColor color = {};
    vec3 tri[3];

    RandomShader(const model &m) : mesh(m) {}

    virtual vec4 vert(const face f, const int v_idx) {
        vertex v = mesh.vertices[f.vertex_idx[v_idx]];
        vec4 gl_Position = ModelView * vec4(v.x(), v.y(), v.z(), 1);
        tri[v_idx] = gl_Position.xyz();
        return Perspective * gl_Position;
    }

    virtual std::pair<bool, TGAColor> frag(const vec3 bar) const {
        return {false, color};
    }
};

struct PhongShader : IShader {
    const model &mesh;
    vec4 light;
    vec2 varying_uv[3];

    PhongShader(const vec3 l, const model& m) : mesh(m) {
        light = normalized(ModelView * vec4(l.x(), l.y(), l.z(), 0));
    }

    virtual vec4 vert(const face f, const int v_idx) {
        vertex v = mesh.vertices[f.vertex_idx[v_idx]];
        vec4 gl_Position = ModelView * vec4(v.x(), v.y(), v.z(), 1);

        varying_uv[v_idx] = mesh.texcoords[f.vtex_idx[v_idx]];
        

        return Perspective * gl_Position;
    }

    virtual std::pair<bool, TGAColor> frag(const vec3 bar) const {
        vec2 uv = bar[0]*varying_uv[0] + bar[1]*varying_uv[1] + bar[2]*varying_uv[2];
        vec4 n = vec4(normalized(inverse_transpose(ModelView) * mesh.normal(uv).xyz()), 0);
        double intensity = 1.5;
        double ambient = 0.3;

        TGAColor gl_FragColor = sample2D(mesh.diffuse_map, uv);
        double diffuse_intensity = 0.5;
        double diffuse = std::max(0., n.dot(light)) * diffuse_intensity;

        double spec_exp = 10;
        double spec_intensity = 1.;

        vec4 r = normalized(2*n*(n.dot(light)) - light);
        double specular = pow(std::max(0., r.z()), spec_exp) * spec_intensity;
        specular *= (0.5+2*sample2D(mesh.specular_map, uv)[0]/255.);
        for (int c : {0,1,2}) {
            gl_FragColor[c] = std::min<int>(255, gl_FragColor[c]*(ambient + diffuse + specular)*intensity);
        }
        
        return {false, gl_FragColor};
    }
};


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Please input at least one .obj model file" << std::endl;
        return 1;
    }

    constexpr int width  = 800;
    constexpr int height = 800;
    
    TGAImage framebuffer(width, height, TGAImage::RGB, {177, 195, 209, 255});
    TGAImage depthImg(width, height, TGAImage::GRAYSCALE);

    const vec3 light(0,0,1);
    const vec3 eye(-1,0,2);
    const vec3 center(0,0,0);
    const vec3 up(0,1,0);

    lookat(eye, center, up);
    perspective(norm(eye - center));
    viewport(width/16, height/16, width*7/6, height*7/8);
    init_zbuffer(width, height);
    
    std::string filepath = argv[1];

    model mesh;
    mesh.read_obj_file(filepath);
    PhongShader shader(light, mesh);

    // draw triangles
    // iterate over faces
    for (int i = 0; i < mesh.faces.size(); i++) {
        face f = mesh.faces[i];

        Triangle clip;
        for (int i = 0; i < 3; i++) clip[i] = shader.vert(f, i);
        // for (int c = 0; c < 3; c++) shader.color[c] = rand() % 255;

        rasterize(framebuffer, clip, shader);
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
