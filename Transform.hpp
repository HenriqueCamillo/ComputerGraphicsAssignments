#pragma once

#include "Vector3.hpp"
#include <vector>

class Transform {
private:
    bool translationDirty;
    bool rotationDirty;
    bool scaleDirty;
    bool transformationDirty;

    Vector3 position;
    Vector3 rotation;
    Vector3 scale;

    void updateTranslationMatrix();
    void updateRotationMatrices();
    void updateScaleMatrix();
    void updateTransformationMatrix();

    std::vector<float> rotationXMatrix;
    std::vector<float> rotationYMatrix;
    std::vector<float> rotationZMatrix;
    std::vector<float> scaleMatrix;
    std::vector<float> translationMatrix;
    std::vector<float> transformationMatrix;

    std::vector<float> matrixMultiplication(std::vector<float>& m1, int r1, int c1, std::vector<float>& m2, int r2, int c2);

public:
    std::vector<float> getTransformationMatrix();

    Vector3 getPosition();
    Vector3 getRotation();
    Vector3 getScale();

    void setPosition(Vector3 position);
    void setRotation(Vector3 rotation);
    void setScale(Vector3 scale);

    Transform();
    ~Transform();
};