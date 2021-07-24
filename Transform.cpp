#include "Transform.hpp"
#include <cmath>

Transform::Transform() {
    scale = Vector3::one;

    translationDirty = true;
    rotationDirty = true;
    scaleDirty = true;
    transformationDirty = true;

    updateTransformationMatrix();
}

Transform::~Transform() {}

Vector3 Transform::getPosition() { return position; }
Vector3 Transform::getRotation() { return rotation; }
Vector3 Transform::getScale() { return scale; }

void Transform::setPosition(Vector3 value) { 
    if (position == value) {
        return;
    }

    position = value; 
    translationDirty = true;
    transformationDirty = true;
}

void Transform::setRotation(Vector3 value) { 
    if (rotation == value) {
        return;
    }
    
    rotation = value; 
    rotationDirty = true;
    transformationDirty = true;
}

void Transform::setScale(Vector3 value) { 
    if (scale == value) {
        return;
    }

    scale = value; 
    scaleDirty = true;
    transformationDirty = true;
}

std::vector<float> Transform::getTransformationMatrix() {
    updateTransformationMatrix();
    return transformationMatrix;
}

void Transform::updateTranslationMatrix() {
    if (!translationDirty) {
        return;
    }

    translationMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        position.x, position.y, position.z, 1.0f
    };

    translationDirty = false;
}

void Transform::updateRotationMatrices() {
    if (!rotationDirty) {
        return;
    }

    // X
    float cosTheta = cos(rotation.x * (M_PI / 180));
    float senTheta = sin(rotation.x * (M_PI / 180));

    rotationXMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosTheta,  senTheta, 0.0f,
        0.0f, -senTheta, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Y
    cosTheta = cos(rotation.y * (M_PI / 180));
    senTheta = sin(rotation.y * (M_PI / 180));

    rotationYMatrix = {
        cosTheta, 0.0f, -senTheta, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        senTheta, 0.0f, cosTheta, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // Z
    cosTheta = cos(rotation.z * (M_PI / 180));
    senTheta = sin(rotation.z * (M_PI / 180));

    rotationZMatrix = {
        cosTheta,  senTheta, 0.0f, 0.0f,
        -senTheta, cosTheta, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    rotationDirty = false;
}

void Transform::updateScaleMatrix() {
    if (!scaleDirty) {
        return;
    }

    scaleMatrix = {
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };

    scaleDirty = false;
}

void Transform::updateTransformationMatrix() {
    if (!transformationDirty) {
        return;
    }

    updateTranslationMatrix();
    updateRotationMatrices();
    updateScaleMatrix();

    transformationMatrix = translationMatrix;
    transformationMatrix = matrixMultiplication(transformationMatrix, 4, 4, rotationZMatrix, 4, 4);
    transformationMatrix = matrixMultiplication(transformationMatrix, 4, 4, rotationXMatrix, 4, 4);
    transformationMatrix = matrixMultiplication(transformationMatrix, 4, 4, rotationYMatrix, 4, 4);
    transformationMatrix = matrixMultiplication(transformationMatrix, 4, 4, scaleMatrix, 4, 4);

    transformationDirty = false;
}

std::vector<float> Transform::matrixMultiplication(std::vector<float>& m1, int r1, int c1, std::vector<float>& m2, int r2, int c2) {
    if (c1 != r2) {
        return {};
    }

    std::vector<float> m3(r1 * c2);
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                m3[i * r1 + j] += m1[i * r1 + k] * m2[k * r2 + j];
            }
        }
    }

    return m3;
}