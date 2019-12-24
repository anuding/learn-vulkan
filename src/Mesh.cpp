//
// Created by anuding on 2019/12/24.
//

#include "Mesh.h"

void Mesh::init() {
//    vertices = {{{0.0f,  -0.5f, 0.0f},
//                        {1.0f, 0.0f, 0.0f}},
//                {{0.5f,  0.5f,  0.0f},
//                        {0.0f, 1.0f, 0.0f}},
//                {{-0.5f, 0.5f,  0.0f},
//                        {0.0f, 0.0f, 1.0f}}};
}

const std::vector<Vertex> &Mesh::getVertices() const {
    return vertices;
}
