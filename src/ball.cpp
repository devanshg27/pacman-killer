#include "ball.h"
#include "main.h"
#include "physics.h"

Ball::Ball(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 0.01;
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

void Ball::tick() {
    this->position.x -= speed;
    this->shape.centerX = this->position.x;
    this->shape.centerY = this->position.y;
    // this->position.y -= speed;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, 0.4, 0.4 };
    return bbox;
}
