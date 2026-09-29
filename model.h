#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "vec.h"


struct face {
    face(const int v[3], const int vt[3], const int vn[3]) {
        for (int i : {0,1,2}) {
            vertex_idx[i] = v[i];
            vtex_idx[i] = vt[i];
            vnrm_idx[i] = vn[i];
        }
    }
    
    int vertex_idx[3];
    int vtex_idx[3];
    int vnrm_idx[3];
};

struct model {
    std::vector<vertex> vertices;
    std::vector<face> faces;
    std::vector<vec3> vnormals;
    std::vector<vec2> texcoords;
    TGAImage normal_map;
    TGAImage diffuse_map;
    TGAImage specular_map;

    vec4 normal(const vec2& uv) const;

    bool read_obj_file(std::string filepath);
};

bool model::read_obj_file(std::string filepath) {
    std::ifstream read(filepath);
    if (!read.is_open()) {
        std::cerr << "File could not be opened" << std::endl;
        return false;
    }

    std::string line;
    while (getline(read, line)) {
        std::stringstream ss(line);
        std::string type;
        
        ss >> type;
        if (type == "v") {
            double x, y, z;
            ss >> x >> y >> z;
            vertex v(x, y, z);
            vertices.push_back(v);
        } else if (type == "vt") {
            double x, y, w;
            ss >> x >> y >> w;
            vec2 texcoord(x, 1-y);
            texcoords.push_back(texcoord);
        } else if (type == "vn") {
            double x, y, z;
            ss >> x >> y >> z;
            vec3 vnrm(x, y, z);
            vnormals.push_back(vnrm);
        } else if (type == "f") {
            int v[3];
            int vt[3];
            int vn[3];

            char delim;
            int vert, vtex, vnrm;
            int i = 0;
            while (ss >> vert >> delim >> vtex >> delim >> vnrm) {
                v[i] = vert-1;
                vt[i] = vtex-1;
                vn[i] = vnrm-1;
                i++;
            }

            face f{v, vt, vn};
            faces.push_back(f);
        }
    }

    read.close();

    normal_map.read_tga_file(filepath.substr(0, filepath.find(".")) + "_nm.tga");
    diffuse_map.read_tga_file(filepath.substr(0, filepath.find(".")) + "_diff.tga");
    specular_map.read_tga_file(filepath.substr(0, filepath.find(".")) + "_spec.tga");

    return true;
}

vec4 model::normal(const vec2& uv) const {
    TGAColor c = normal_map.get(uv[0] * normal_map.width(), uv[1] * normal_map.height());
    return vec4((double)c[2], (double)c[1], (double)c[0], 0)*2./255. - vec4(1,1,1,0);
}


#endif