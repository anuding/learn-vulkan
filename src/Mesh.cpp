//
// Created by anuding on 2019/12/24.
//

#include "Mesh.h"

void Mesh::init() {
}

const std::vector<Vertex> &Mesh::getVertices() const {
    return vertices;
}

void Mesh::setVertices(const std::vector<Vertex> &vertices) {
    Mesh::vertices = vertices;
}

const std::vector<uint16_t> &Mesh::getIndices() const {
    return indices;
}

void Mesh::setIndices(const std::vector<uint16_t> &indices) {
    Mesh::indices = indices;
}
