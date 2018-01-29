#include "pool.h"
#include "main.h"

Pool::Pool(float x, float y, color_t grassColor, color_t groundColor, color_t waterColor) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;

    this->restitution = 0;

    static const GLfloat vertex_buffer_data1[] = {
        -2.5, 0.75,  0,
        -2.5, -10, 0,
        2.5,  -10, 0,
        2.5, 0.75, 0,
    };
    static const GLfloat vertex_buffer_data2[] = {
        -2.5, 1, 0,
        -2.5, 0.75, 0,
        2.5,  0.75, 0,
        2.5,  1, 0,
    };

    const int sides = 50;
    GLfloat vertex_buffer_data_water[6 + 3*sides]; // Triangle fan requires N+2 vertices for N triangles
    float r = 1.6;
    float poolWidth = 1.5;
    float poolDepth = r - sqrt(r*r - poolWidth*poolWidth);
    float theta = 2*(acos((r - poolDepth)/r)/sides);
    float startingAngle = (1.5*PI) - acos((r - poolDepth)/r);

    vertex_buffer_data_water[0] = 0.0f;
    vertex_buffer_data_water[1] = 1.0f;
    vertex_buffer_data_water[2] = 0.0f;
    for(int i=0; i<=sides; ++i) {
        vertex_buffer_data_water[3+3*i] = r*cos(startingAngle + i*theta);
        vertex_buffer_data_water[4+3*i] = 1 + r*sin(startingAngle + i*theta) + r - poolDepth;
        vertex_buffer_data_water[5+3*i] = 0.0f;
    }

    this->groundObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data1, groundColor, GL_FILL);
    this->grassObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data2, grassColor, GL_FILL);
    this->waterObject = create3DObject(GL_TRIANGLE_FAN, sides+2, vertex_buffer_data_water, waterColor, GL_FILL);

    this->shape = PoolHull(x, y + 1 + r - poolDepth, r, poolWidth);
}

void Pool::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->grassObject);
    draw3DObject(this->groundObject);
    draw3DObject(this->waterObject);
}