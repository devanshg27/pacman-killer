#include "trampoline.h"
#include "main.h"

Trampoline::Trampoline(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data_trampoline[] = {
        -2.5, 1, 0,
        -2.5, 0.75, 0,
        2.5,  0.75, 0,
        2.5,  1, 0,
    };

    this->leftObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_trampoline, color, GL_FILL);
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
}

void Trampoline::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}
