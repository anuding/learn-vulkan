//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include "Vertex.h"
#include <vector>
class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<uint16_t > indices;
public:
    const std::vector<uint16_t> &getIndices() const;

    void setIndices(const std::vector<uint16_t> &indices);

public:
    const std::vector<Vertex> &getVertices() const;

    void setVertices(const std::vector<Vertex> &vertices);

public:

    void init();
};


#endif //ENGINE_MESH_H
