#include "main.h"
#include "physics.h"
#ifndef GROUND_H
#define GROUND_H

class Ground {
public:
    Ground() {}
    Ground(float x, float y, color_t color1, color_t color2);
    glm::vec3 position;
    Rectangle shape;
    float rotation;
    void draw(glm::mat4 VP);
private:
    VAO *grassObject;
    VAO *groundObject;
};

#endif // Ground_H
