#include "GameObject.hpp"

std::set<GameObject*> GameObject::all;

GameObject::GameObject() {
    renderer = Renderer(&transform);
    all.insert(this);
}

GameObject::~GameObject() {
    all.erase(this);
}

const std::set<GameObject*>* GameObject::getAll() {
    return &all;
}