#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <cstdint>
#include "canvas.hpp"
#include "camera.hpp"

class Model {
public:
    Model(std::string objfile);
    float maxOriginDistance() const;

    template <uint8_t S>
    void render(Canvas<S> & canvas, Camera & camera);

private:
    struct FaceIndex {
        uint32_t v = 0;
        uint32_t vt = 0;
        uint32_t vn = 0;
    };

    typedef std::vector<FaceIndex> Face;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> vertex_normals;
    std::vector<glm::vec3> vertex_textures;
    std::vector<Face> faces; 
    float max_origin_distance = 0.0f;
};