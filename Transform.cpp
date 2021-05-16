#include "Transform.hpp"

Transform::Transform(Vector3 position, Vector3 rotation, Vector3 scale) : 
    position(position), rotation(rotation), scale(scale) 
{}

Transform::~Transform() {}