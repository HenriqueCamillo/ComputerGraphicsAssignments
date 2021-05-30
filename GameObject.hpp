#pragma once

#include "Transform.hpp"
#include "Renderer.hpp"
#include <set>

class GameObject {
private:
    static std::set<GameObject*> all;

public:
    static const std::set<GameObject*>* getAll();

    Transform transform;
    Renderer renderer;

    GameObject();
    ~GameObject();
};

