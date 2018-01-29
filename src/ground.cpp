#include "ground.h"
#include "main.h"

Ground::Ground(float x, float y, color_t grassColor, color_t groundColor) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->restitution = 0.4;
    this->shape = Rectangle(x-2.5, y-10, x+2.5, y+1, this->rotation);
    static const GLfloat vertex_buffer_data_ground[] = {
        -2.5, 0.75,  0,
        -2.5, -10, 0,
        2.5,  -10, 0,
        2.5, 0.75, 0,
    };
    static const GLfloat vertex_buffer_data_grass[] = {
        -2.5, 1, 0,
        -2.5, 0.75, 0,
        2.5,  0.75, 0,
        2.5,  1, 0,
    };

    this->groundObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_ground, groundColor, GL_FILL);
    this->grassObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_grass, grassColor, GL_FILL);
}

void Ground::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->grassObject);
    draw3DObject(this->groundObject);
}
