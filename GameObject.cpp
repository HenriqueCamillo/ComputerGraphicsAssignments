#include "GameObject.hpp"

std::set<GameObject*> GameObject::all;

GameObject::GameObject() {
    all.insert(this);
}

GameObject::~GameObject() {
    all.erase(this);
}

const std::set<GameObject*>* GameObject::getAll() {
    return &all;
}