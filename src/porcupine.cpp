#include "porcupine.h"
#include "main.h"

Porcupine::Porcupine(float x, float y, color_t color, float x_add, float minX, float maxX) {
    this->position = glm::vec3(x, y, 0);
    this->restitution = 0.1;
    this->x_add = x_add;
    this->minX = minX;
    this->maxX = maxX;
    this->isAlive = false;
    float width = 0.25;
    float height = (sqrt(3) * width / 2.0);
    const int num = 7;
    GLfloat vertex_buffer_data_porcupine[9*num]; // Triangles requires 3*N vertices for N triangles
    for(int i=0; i<num; ++i) {
        vertex_buffer_data_porcupine[9*i + 0] = i*width;
        vertex_buffer_data_porcupine[9*i + 1] = 0;
        vertex_buffer_data_porcupine[9*i + 2] = 0;

        vertex_buffer_data_porcupine[9*i + 3] = i*width + width/2;
        vertex_buffer_data_porcupine[9*i + 4] = height;
        vertex_buffer_data_porcupine[9*i + 5] = 0;

        vertex_buffer_data_porcupine[9*i + 6] = i*width + width;
        vertex_buffer_data_porcupine[9*i + 7] = 0;
        vertex_buffer_data_porcupine[9*i + 8] = 0;
    }
    this->shape = Rectangle(x+width/4, y, x+num*width - width/4, y+height, 0);
    this->object = create3DObject(GL_TRIANGLES, 3*num, vertex_buffer_data_porcupine, color, GL_FILL);
}

void Porcupine::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (0.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Porcupine::move() {
    if(this->position.x + this->x_add < this->minX or this->position.x + this->x_add > this->maxX) this->x_add *= -1;
    this->position = glm::vec3(this->position.x + this->x_add, this->position.y, 0);
    this->shape.xLeft += this->x_add;
    this->shape.xRight += this->x_add;
}
