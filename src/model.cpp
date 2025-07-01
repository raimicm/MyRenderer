#include "model.hpp"
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
