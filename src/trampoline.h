#include "main.h"
#include "physics.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    Rectangle shape;
    float restitution;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *leftObject;
    VAO *rightObject;
    VAO *middleObject;
};

#endif // Trampoline_H
