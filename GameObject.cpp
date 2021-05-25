#include "GameObject.hpp"

std::unordered_set<GameObject*> GameObject::all;

GameObject::GameObject() {
    all.insert(this);
}

GameObject::~GameObject() {
    all.erase(this);
}

const std::unordered_set<GameObject*>& GameObject::getAll() const {
    return all;
}