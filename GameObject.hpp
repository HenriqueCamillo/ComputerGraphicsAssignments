#pragma once

#include "Transform.hpp"
#include "Renderer.hpp"
#include <unordered_set>

class GameObject {
private:
    static std::unordered_set<GameObject*> all;


public:
    const std::unordered_set<GameObject*>& getAll() const;

    Transform transform;
    Renderer renderer;

    GameObject();
    ~GameObject();
};

