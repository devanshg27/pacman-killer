#include "magnet.h"
#include "main.h"

Magnet::Magnet(float x, float y, color_t color1, color_t color2, float rotation) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rotation;
    float r1 = 0.4, r2 = 0.22;
    const GLfloat vertex_buffer_data_magnet_left[] = {
        0.9, -r1, 0,
        1.4, -r1, 0,
        1.4, -r2, 0,
        0.9, -r2, 0,
    };

    const GLfloat vertex_buffer_data_magnet_right[] = {
        0.9, r1, 0,
        1.4, r1, 0,
        1.4, r2, 0,
        0.9, r2, 0,
    };

    const int sides = 50;
    GLfloat vertex_buffer_data_magnet_middle[6 + 3*sides];
    float theta = 2*PI/sides;
    float startingAngle = PI/2;

    for(int i=0; i<=sides+1; ++i) {
        vertex_buffer_data_magnet_middle[0+3*i] = 0.9 + (1.2 * ((i&1) ? r1: r2)*cos(startingAngle + (i/2)*theta));
        vertex_buffer_data_magnet_middle[1+3*i] = ((i&1) ? r1: r2)*sin(startingAngle + (i/2)*theta);
        vertex_buffer_data_magnet_middle[2+3*i] = 0.0f;
    }

    this->leftObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_magnet_left, color2, GL_FILL);
    this->rightObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_magnet_right, color2, GL_FILL);
    this->middleObject = create3DObject(GL_TRIANGLE_STRIP, sides+2, vertex_buffer_data_magnet_middle, color1, GL_FILL);
}

void Magnet::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->leftObject);
    draw3DObject(this->rightObject);
    draw3DObject(this->middleObject);
}

void Magnet::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
