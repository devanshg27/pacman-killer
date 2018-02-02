#include "main.h"
#include "physics.h"

#ifndef DISPLAY_H
#define DISPLAY_H

class Display {
public:
    Display() {}
    Display(float x, float y, color_t color, int mask);
    glm::vec3 position;
    int mask;
    void draw(glm::mat4 VP);
private:
    VAO *object[9];
};

#endif // Display_H
