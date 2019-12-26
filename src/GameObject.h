//
// Created by anuding on 2019/12/24.
//

#ifndef ENGINE_GAMEOBJECT_H
#define ENGINE_GAMEOBJECT_H

#include "Mesh.h"
#include <vector>
class GameObject {
public:
    Mesh mesh;

    const Mesh &getMesh() const;

    void setMesh(const Mesh &mesh);
};


#endif //ENGINE_GAMEOBJECT_H
