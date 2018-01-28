#include "main.h"

#ifndef TRAMPOLINE_H
#define TRAMPOLINE_H

class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *leftObject;
    VAO *rightObject;
    VAO *middleObject;
};

#endif // Trampoline_H
