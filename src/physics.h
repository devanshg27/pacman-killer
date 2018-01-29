#include <utility>
#include "main.h"

#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
public:
    Rectangle() {}
    Rectangle(float _xLeft, float _yBottom, float _xRight, float _yTop, float _rotation);
    float xLeft, yBottom, xRight, yTop, rotation;
    float cosAng, sinAng; // Precomputed for rotations
};

class Circle {
public:
    Circle() {}
    Circle(float _centerX, float _centerY, float _radius);
    float centerX, centerY, radius;
};

class PoolHull {
public:
    PoolHull() {}
    PoolHull(float _centerX, float _centerY, float _radius, float _xRange);
    float centerX, centerY, radius, xRange, sinAng;
};

struct Vec2D{
	float x, y;
	Vec2D(float _x=0, float _y=0);
};

Vec2D operator+(const Vec2D &a,const Vec2D &b);
Vec2D operator-(const Vec2D &a,const Vec2D &b);
float operator*(const Vec2D &a,const Vec2D &b);
Vec2D operator/(const Vec2D &a, const float k);
float operator^(const Vec2D &a,const Vec2D &b);

#endif // Rectangle_H

std::pair<std::pair<bool, float>,Vec2D> detect_collision(Rectangle &rec, Circle &cir);
std::pair<std::pair<bool, float>,Vec2D> detect_collision(Circle &cir, Rectangle &rec);
std::pair<std::pair<bool, float>,Vec2D> detect_collision(PoolHull &pool, Circle &cir);
std::pair<std::pair<bool, float>,Vec2D> detect_collision(Circle &cir, PoolHull &pool);