#include "main.h"
#include "physics.h"

#ifndef POOL_H
#define POOL_H

class Pool {
public:
    Pool() {}
    Pool(float x, float y, color_t color1, color_t color2, color_t color3);
    glm::vec3 position;
    PoolHull shape;
    float rotation;
    float restitution;
    void draw(glm::mat4 VP);
private:
    VAO *grassObject;
    VAO *waterObject;
    VAO *groundObject;
};

#endif // Pool_H
