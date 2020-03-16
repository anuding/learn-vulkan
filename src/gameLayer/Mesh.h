//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include "Vertex.h"
#include <vector>
#include "Component.h"

class Mesh : public Componenet {
public:
	std::vector<Vertex> vertices;
	std::vector<uint16_t > indices;
	Mesh();
	~Mesh();
};


#endif //ENGINE_MESH_H
