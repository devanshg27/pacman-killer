#include "main.h"

#ifndef POOL_H
#define POOL_H

class Pool {
public:
    Pool() {}
    Pool(float x, float y, color_t color1, color_t color2, color_t color3);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *grassObject;
    VAO *waterObject;
    VAO *groundObject;
};

#endif // Pool_H
