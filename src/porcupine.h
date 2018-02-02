#include "main.h"
#include "physics.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H

class Porcupine {
public:
    Porcupine() {}
    Porcupine(float x, float y, color_t color, float x_add, float minX, float maxX);
    float x_add, minX, maxX;
    glm::vec3 position;
    bool isAlive;
    Rectangle shape;
    float restitution;
    void draw(glm::mat4 VP);
    void move();
private:
    VAO *object;
};

#endif // Porcupine_H
