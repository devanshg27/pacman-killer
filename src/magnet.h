#include "main.h"
#include "physics.h"

#ifndef MAGNET_H
#define MAGNET_H

class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color1, color_t color2, float rotation);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
private:
    VAO *leftObject;
    VAO *rightObject;
    VAO *middleObject;
};

#endif // Magnet_H
