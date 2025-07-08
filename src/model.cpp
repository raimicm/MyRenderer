#include "model.hpp"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <cstdlib>

#include "canvas.hpp"
#include "camera.hpp"

Model::Model(std::string objfile) {
    std::ifstream objFile(objfile);

    if (!objFile.is_open()) {
        throw std::runtime_error("Error opening file");
    }

    std::string line;
    int nlines = 1;
    while (std::getline(objFile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 v;
            if (!(iss >> v.x >> v.y >> v.z))
                throw std::runtime_error("Invalid format at line " + std::to_string(nlines));
            vertices.push_back(v);
        }
        if (prefix == "vn") {
            glm::vec3 vn;
            if (!(iss >> vn.x >> vn.y >> vn.z))
                throw std::runtime_error("Invalid format at line " + std::to_string(nlines));
            vertex_normals.push_back(vn);
        }
        if (prefix == "vt") {
            glm::vec3 vt;
            if (!(iss >> vt.x >> vt.y >> vt.z))
                throw std::runtime_error("Invalid format at line " + std::to_string(nlines));
            vertex_textures.push_back(vt);
        }
        if (prefix == "f") {
            Face face;
            std::string token;

            auto parseFaceIndex = [&token]() {
                FaceIndex fi;
                char sep;
                std::istringstream iss(token);
                
                if (!(iss >> fi.v >> sep >> fi.vt >> sep >> fi.vn))
                    throw std::runtime_error("Invalid face index format: " + token);
                
                fi.v--;
                fi.vt--;
                fi.vn--;

                return fi;
            };

            while (iss >> token) {
                try {
                    face.push_back(parseFaceIndex());
                } catch (const std::exception& e) {
                    throw std::runtime_error(std::string(e.what()) + " in face line: " + line);
                }
            }
            faces.push_back(face);
        }
        nlines++;
    }
}

template <uint8_t S>
void Model::render(Canvas<S> & canvas, Camera & camera) {
    for (Face face : faces) {
        glm::mat4 view = camera.getViewMatrix(); 
        glm::mat4 projection = camera.getProjectionMatrix();
        
        std::array<glm::vec3, 4> v_i;
        std::array<glm::vec3, 4> vn_i;
        std::array<glm::vec3, 4> vt_i;

        for (int i = 0; i < face.size(); i++) {
            glm::vec3 v3 = vertices[face[i].v];
            glm::vec4 v4(v3.x, v3.y, v3.z, 1.0f);
            v4 = projection * view * v4;
            v_i[i] = glm::vec3(v4.x/v4.w, v4.y/v4.w, v4.z/v4.w);

            v3 = vertex_normals[face[i].vn];
            v4 = glm::vec4(v3.x, v3.y, v3.z, 0.0f);
            vn_i[i] = projection * view * v4;

            vt_i[i] = vertex_textures[face[i].vt];
        }

        float top = static_cast<float>(canvas.height());
        float left = static_cast<float>(canvas.width());
        float bottom = 0.0f;
        float right = 0.0f;

        std::array<glm::vec3, 4> canvas_coordinates; 
        for (int i = 0; i < face.size(); i++) {
            canvas_coordinates[i] = glm::vec3(
                (v_i[i].x + 0.5f) * static_cast<float>(canvas.width()),
                (-v_i[i].y + 0.5f) * static_cast<float>(canvas.height()),
                v_i[i].z
            );

            top = std::min(top, canvas_coordinates[i].y);
            left = std::min(left, canvas_coordinates[i].x);
            bottom = std::max(bottom, canvas_coordinates[i].y);
            right = std::max(right, canvas_coordinates[i].x);
        }

        // assume that all faces are trianglular for now

        glm::vec2 p1(canvas_coordinates[0].x, canvas_coordinates[0].y);
        glm::vec2 p2(canvas_coordinates[1].x, canvas_coordinates[1].y);
        glm::vec2 p3(canvas_coordinates[2].x, canvas_coordinates[2].y);

        uint8_t col = std::rand() % 55 + 200; // will be replaced with texture

        for (float xp = left; xp <= right; xp++) {
            for (float yp = top; yp <= bottom; yp++) {

                float step = 1.0f / S;
                for (float x = xp + 0.5f * step; x < xp + 1.0f; x += step) {
                    for (float y = yp + 0.5f * step; y < yp + 1.0f; y += step) {

                        // barycentric coordinates
                        float d = ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
                        float a = ((p2.y - p3.y)*(x - p3.x) + (p3.x - p2.x)*(y - p3.y)) / d;
                        float b = ((p3.y - p1.y)*(x - p3.x) + (p1.x - p3.x)*(y - p3.y)) / d;
                        float c = 1.0f - a - b;

                        if (a >= 0.0f && b >= 0.0f && c >= 0.0f) {
                            double z = (a * canvas_coordinates[0].z + b * canvas_coordinates[1].z + c * canvas_coordinates[2].z);
                            
                            canvas.addSample(x, y, col, col, col, z);
                        }
                    }
                }
            }
        }
    }
}

// explicitly instantiated templates
template void Model::render<1>(Canvas<1>&, Camera&);
template void Model::render<2>(Canvas<2>&, Camera&);
template void Model::render<3>(Canvas<3>&, Camera&);
template void Model::render<4>(Canvas<4>&, Camera&);