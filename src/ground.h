#include "main.h"

#ifndef GROUND_H
#define GROUND_H

class Ground {
public:
    Ground() {}
    Ground(float x, float y, color_t color1, color_t color2);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *grassObject;
    VAO *groundObject;
};

#endif // Ground_H
