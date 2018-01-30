#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->restitution = 1.37;
    this->shape = Rectangle(x-1.2, y+0.9, x+1.2, y+0.91, this->rotation);
    const GLfloat vertex_buffer_data_trampoline_left[] = {
        -1, 0.75, 0,
        -0.8, 0.75, 0,
        -0.8, 0, 0,
        -1, 0, 0,
    };

    const GLfloat vertex_buffer_data_trampoline_right[] = {
        1, 0.75, 0,
        0.8, 0.75, 0,
        0.8, 0, 0,
        1, 0, 0,
    };

    const int sides = 50;
    GLfloat vertex_buffer_data_trampoline_middle[6 + 3*sides]; // Triangle fan requires N+2 vertices for N triangles
    float r = 1.7;
    float middleWidth = 1.2;
    float middleDepth = r - sqrt(r*r - middleWidth*middleWidth);
    float theta = 2*(acos((r - middleDepth)/r)/sides);
    float startingAngle = (1.5*PI) - acos((r - middleDepth)/r);

    vertex_buffer_data_trampoline_middle[0] = 0.0f;
    vertex_buffer_data_trampoline_middle[1] = 0.9f;
    vertex_buffer_data_trampoline_middle[2] = 0.0f;
    for(int i=0; i<=sides; ++i) {
        vertex_buffer_data_trampoline_middle[3+3*i] = r*cos(startingAngle + i*theta);
        vertex_buffer_data_trampoline_middle[4+3*i] = 0.9 + r*sin(startingAngle + i*theta) + r - middleDepth;
        vertex_buffer_data_trampoline_middle[5+3*i] = 0.0f;
    }

    this->leftObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_trampoline_left, color, GL_FILL);
    this->rightObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_trampoline_right, color, GL_FILL);
    this->middleObject = create3DObject(GL_TRIANGLE_FAN, sides+2, vertex_buffer_data_trampoline_middle, color, GL_FILL);
}

void Trampoline::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->leftObject);
    draw3DObject(this->middleObject);
    draw3DObject(this->rightObject);
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
