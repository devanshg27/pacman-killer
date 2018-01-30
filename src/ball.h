#include "main.h"
#include "physics.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float _mass, float x, float y, color_t color);
    glm::vec3 position;
    Circle shape;
    float rotation;

    Vec2D velocity, acceleration;
    float angularVelocity, angularAcceleration;
    float mass, momentOfInertia;

    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void handleCollision(Vec2D normal, float restitution, float correction);
    void tick(float dt, bool inWater);

    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // BALL_H
