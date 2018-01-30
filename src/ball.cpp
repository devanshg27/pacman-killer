#include "ball.h"
#include "main.h"
#include "physics.h"

float sign_function(float a) {
    if(fabs(a) <= 0.0001) return 0;
    if(a < 0) return -1;
    return 1;
}

Ball::Ball(float _mass, float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->mass = _mass;
    this->momentOfInertia = (_mass * 0.2 * 0.2) / 2; // I = (m*r^2)/2
    this->velocity = Vec2D(0, 0);
    this->acceleration = Vec2D(0, -5);
    angularVelocity = angularAcceleration = 0;

    const int sides = 50;

    GLfloat vertex_buffer_data[6 + 3*sides]; // Triangle fan requires N+2 vertices for N triangles

    vertex_buffer_data[0] = 0.0f;
    vertex_buffer_data[1] = 0.0f;
    vertex_buffer_data[2] = 0.0f;
    for(int i=0; i<=sides; ++i) {
        vertex_buffer_data[3+3*i] = 0.2f*cos(i*2*PI/sides);
        vertex_buffer_data[4+3*i] = 0.2f*sin(i*2*PI/sides);
        vertex_buffer_data[5+3*i] = 0.0f;
    }

    this->shape = Circle(x, y, 0.2);

    this->object = create3DObject(GL_TRIANGLE_FAN, sizeof(vertex_buffer_data)/(sizeof(vertex_buffer_data[0])*3), vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick(float dt, bool inWater) {
    Vec2D tempFriction = (Vec2D(sign_function(this->velocity.x), sign_function(this->velocity.y))/(inWater ? 1: 4));
    this->acceleration = this->acceleration - tempFriction;

    this->velocity = this->velocity + (this->acceleration/(1/dt));
    this->acceleration = this->acceleration + tempFriction;
    this->angularVelocity += this->angularAcceleration * dt;
    this->position.x += (inWater ? 0.5 : 1) * this->velocity.x * dt;
    this->position.y += (inWater ? 0.5 : 1) * this->velocity.y * dt;
    this->rotation += this->angularVelocity * dt;

    this->velocity.x = std::min(this->velocity.x, 10.0f);
    this->velocity.x = std::max(this->velocity.x, -10.0f);

    this->velocity.y = std::min(this->velocity.y, 8.0f);

    this->shape.centerX = this->position.x;
    this->shape.centerY = this->position.y;
}

void Ball::handleCollision(Vec2D normal, float restitution, float correction) {
    normal = normal/sqrt(normal * normal); // Make a unit vector

    float velAlongNormal = -(this->velocity * normal);
    if(velAlongNormal <= 0) return;

    float j = (1 + restitution) * velAlongNormal;

    Vec2D impulse = normal / (1 / j);
    this->velocity = this->velocity + impulse;

    if(correction < 0.0005) return;
    this->position.x += 0.7*normal.x*correction;
    this->position.y += 0.7*normal.y*correction;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
