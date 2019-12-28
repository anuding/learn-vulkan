//
// Created by anuding on 2019/12/24.
//

#include "GameObject.h"

const Mesh &GameObject::getMesh() const {
    return mesh;
}

void GameObject::setMesh(const Mesh &mesh) {
    GameObject::mesh = mesh;
}
