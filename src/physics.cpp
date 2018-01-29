#include <utility>
#include <algorithm>
#include "physics.h"
#include "main.h"

float squaredDis(float x1, float y1, float x2, float y2) {
    return (x1 - x2) * (x1 -x2) + (y1 - y2) * (y1 - y2);
}

Rectangle::Rectangle(float _xLeft, float _yBottom, float _xRight, float _yTop, float _rotation) {
    this->xLeft = _xLeft;
    this->yBottom = _yBottom;
    this->xRight = _xRight;
    this->yTop = _yTop;
    this->rotation = (_rotation * PI) / 180.0f;
    this->cosAng = cos(this->rotation);
    this->sinAng = sin(this->rotation);
}

Circle::Circle(float _centerX, float _centerY, float _radius) {
    this->centerX = _centerX;
    this->centerY = _centerY;
    this->radius = _radius;
}

PoolHull::PoolHull(float _centerX, float _centerY, float _radius, float _xRange) {
    this->centerX = _centerX;
    this->centerY = _centerY;
    this->radius = _radius;
    this->xRange = _xRange;
    this->sinAng = (xRange/radius);
}

Vec2D::Vec2D(float _x, float _y) : x(_x), y(_y) {
}

Vec2D operator+(const Vec2D &a,const Vec2D &b) {
    return Vec2D(a.x+b.x,a.y+b.y);
}

Vec2D operator-(const Vec2D &a,const Vec2D &b) {
    return Vec2D(a.x-b.x,a.y-b.y);
}

float operator*(const Vec2D &a,const Vec2D &b) {
    return (a.x*b.x)+(a.y*b.y);
}

Vec2D operator/(const Vec2D &a, const float k) {
    return Vec2D(a.x/k, a.y/k);
}

float operator^(const Vec2D &a,const Vec2D &b) {
    return (a.x*b.y)-(b.x*a.y);
}

std::pair<bool, Vec2D> detect_collision(Rectangle &rec, Circle &cir) {
    float tempCenterX = (cir.centerX - rec.xLeft) * rec.cosAng + (cir.centerY - rec.yBottom) * rec.sinAng + rec.xLeft;
    float tempCenterY = (-(cir.centerX - rec.xLeft) * rec.sinAng) + (cir.centerY - rec.yBottom) * rec.cosAng + rec.yBottom;
    float nearestX = tempCenterX, nearestY = tempCenterY;

    nearestX = std::max(nearestX, rec.xLeft);
    nearestX = std::min(nearestX, rec.xRight);
    nearestY = std::max(nearestY, rec.yBottom);
    nearestY = std::min(nearestY, rec.yTop);

    std::pair<bool, Vec2D> returnVal;
    returnVal.first = (squaredDis(nearestX, nearestY, tempCenterX, tempCenterY)) <= cir.radius * cir.radius;

    float tempNearestX = (nearestX - rec.xLeft) * rec.cosAng + (nearestY - rec.yBottom) * (-rec.sinAng) + rec.xLeft;
    float tempNearestY = (nearestX - rec.xLeft) * rec.sinAng + (nearestY - rec.yBottom) * rec.cosAng + rec.yBottom;
    returnVal.second = Vec2D(tempNearestX, tempNearestY) - Vec2D(cir.centerX, cir.centerY);

    return returnVal;
}

std::pair<bool, Vec2D> detect_collision(Circle &cir, Rectangle &rec) {
    std::pair<bool, Vec2D> temp = detect_collision(rec, cir);
    temp.second = temp.second/-1;
    return temp;
}

std::pair<bool, Vec2D> detect_collision(PoolHull &pool, Circle &cir) {
    if(cir.centerY >= pool.centerY) return {false, {-1, -1}};
    if(fabs(cir.centerX - pool.centerX) >= pool.xRange - cir.radius * pool.sinAng) return {false, {-1, -1}};
    if(squaredDis(cir.centerX, cir.centerY, pool.centerX, pool.centerY) < (pool.radius - cir.radius) * (pool.radius - cir.radius)) return {false, {-1, -1}};

    Vec2D retNormal = Vec2D(cir.centerX, cir.centerY) - Vec2D(pool.centerX, pool.centerY);

    return {true, retNormal};
}

std::pair<bool, Vec2D> detect_collision(Circle &cir, PoolHull &pool) {
    std::pair<bool, Vec2D> temp = detect_collision(pool, cir);
    temp.second = temp.second/-1;
    return temp;
}