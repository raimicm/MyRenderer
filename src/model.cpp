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
            max_origin_distance = std::fmax(max_origin_distance, v.length());
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


            while (iss >> token) {
                uint8_t sep_count = 0;
                for (int i = 0; i < token.length(); i++) {
                    if (token.at(i) == '/')
                        sep_count++;
                }

                FaceIndex fi;
                char sep;
                std::istringstream iss(token);

                switch (sep_count) {
                    case 0:
                        if (!(iss >> fi.v))
                            throw std::runtime_error("Invalid face index format: " + token);
                        break;
                    case 1:
                        if (!(iss >> fi.v >> sep >> fi.vt))
                            throw std::runtime_error("Invalid face index format: " + token);
                        if (fi.vt > vertex_textures.size())
                            throw std::runtime_error("Invalid face index: vertex texture with index " + std::to_string(fi.vt) + " does not exist.");
                        fi.vt--;
                        break;
                    case 2:
                        if (!(iss >> fi.v >> sep))
                            throw std::runtime_error("Invalid face index format: " + token);
                        if (iss.peek() == '/') {
                            if (!(iss >> sep >> fi.vn))
                                throw std::runtime_error("Invalid face index format: " + token);
                        } else {
                            if (!(iss >> fi.vt >> sep >> fi.vn)) {
                                throw std::runtime_error("Invalid face index format: " + token);
                            }
                            if (fi.vt > vertex_textures.size())
                                throw std::runtime_error("Invalid face index: vertex texture with index " + std::to_string(fi.vt) + " does not exist.");
                            fi.vt--;
                        }
                        if (fi.vn > vertex_normals.size()) 
                            throw std::runtime_error("Invalid face index: vertex normal with index " + std::to_string(fi.vn) + " does not exist.");
                        fi.vn--;
                        break;
                    default:
                        throw std::runtime_error("Invalid face index format: " + token);
                }
                if (fi.v > vertices.size())
                    throw std::runtime_error("Invalid face index: vertex with index " + std::to_string(fi.v) + " does not exist.");
                fi.v--;

                face.push_back(fi);
            }
            faces.push_back(face);
        }
        nlines++;
    }
}

float Model::maxOriginDistance() const {
    return max_origin_distance;
}

template <uint8_t S>
void Model::render(Canvas<S> & canvas, Camera & camera) {
    for (Face face : faces) {
        glm::mat4 view = camera.getViewMatrix(); 
        glm::mat4 projection = camera.getProjectionMatrix();
        
        std::array<glm::vec3, 3> view_v;
        std::array<glm::vec3, 3> projected_v;
        std::array<glm::vec3, 3> view_vn;

        for (int i = 0; i < face.size(); i++) {
            glm::vec4 v4(vertices[face[i].v], 1.0f);
            v4 = view * v4;
            view_v[i] = glm::vec3(v4) / v4.w;
            v4 = projection * v4;
            projected_v[i] = glm::vec3(v4) / v4.w;

            if (vertex_normals.size() > 0) {
                v4 = glm::vec4(vertex_normals[face[i].vn], 0.0f);
                view_vn[i] = glm::vec3(view * v4);
            }
        }

        float top = static_cast<float>(canvas.height());
        float left = static_cast<float>(canvas.width());
        float bottom = 0.0f;
        float right = 0.0f;

        std::array<glm::vec3, 3> canvas_coordinates; 
        for (int i = 0; i < face.size(); i++) {
            canvas_coordinates[i] = glm::vec3(
                (projected_v[i].x + 0.5f) * static_cast<float>(canvas.width()),
                (-projected_v[i].y + 0.5f) * static_cast<float>(canvas.height()),
                projected_v[i].z
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

        double col1 = fmax(10.0f, 255.0f * glm::dot(glm::normalize(-view_v[0]), glm::normalize(view_vn[0])));
        double col2 = fmax(10.0f, 255.0f * glm::dot(glm::normalize(-view_v[1]), glm::normalize(view_vn[1])));
        double col3 = fmax(10.0f, 255.0f * glm::dot(glm::normalize(-view_v[2]), glm::normalize(view_vn[2])));

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
                            uint8_t col = 255;
                            if (vertex_normals.size() > 0)
                                col = (uint8_t) std::round(a * col1 + b * col2 + c * col3);
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