#include "main.h"
#include "physics.h"

#ifndef PORCUPINE_H
#define PORCUPINE_H

class Porcupine {
public:
    Porcupine() {}
    Porcupine(float x, float y, color_t color);
    glm::vec3 position;
    Rectangle shape;
    float restitution;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *object;
};

#endif // Porcupine_H
