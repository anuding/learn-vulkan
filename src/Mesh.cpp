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
