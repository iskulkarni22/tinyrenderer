#include <iostream>
#include <fstream>
#include <sstream>
#include "vec.h"


struct face {
    face(const int v1, const int v2, const int v3) : vertex_idx{v1, v2, v3} {}
    
    int vertex_idx[3];
};

struct model {
    std::vector<vertex> vertices;
    std::vector<face> faces;

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
        } else if (type == "f") {
            std::string v1, v2, v3;
            ss >> v1 >> v2 >> v3;

            int v1_idx, v2_idx, v3_idx;
            v1_idx = std::stoi(v1.substr(0, v1.find("/"))) - 1;
            v2_idx = std::stoi(v2.substr(0, v2.find("/"))) - 1;
            v3_idx = std::stoi(v3.substr(0, v3.find("/"))) - 1;

            face f(v1_idx, v2_idx, v3_idx);
            faces.push_back(f);
        }
    }

    read.close();
    return true;
}
