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
public:
    const std::vector<Vertex> &getVertices() const;

public:

    void init();
};


#endif //ENGINE_MESH_H
