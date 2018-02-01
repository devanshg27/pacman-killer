#include "main.h"
#include "physics.h"

#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y, color_t color, float speed, float rotation, bool hasPlank);
    glm::vec3 position;
    float speed;
    float restitution, rotation;
    Rectangle shape, enemyBall;
    void draw(glm::mat4 VP);
    void move();
private:
    VAO *plankObject, *ballObject;
};

#endif // Enemy_H
