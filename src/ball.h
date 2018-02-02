#include "main.h"
#include "physics.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color);
    glm::vec3 position;
    Circle shape;
    float rotation;

    Vec2D velocity, acceleration;
    float angularVelocity, angularAcceleration;

    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void handleCollision(Vec2D normal, float restitution, float correction);
    void tick(float dt, bool inWater);

private:
    VAO *object;
};

#endif // BALL_H
