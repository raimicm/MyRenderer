#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstdint>

class Model {
public:
    Model(std::string objfile);
private:
    struct FaceIndex {
        uint32_t v;
        uint32_t vt;
        uint32_t vn;
    };

    typedef std::vector<FaceIndex> Face;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertex_normals;
    std::vector<glm::vec3> vertex_textures;
    std::vector<Face> faces; 
};