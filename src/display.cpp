#include "display.h"
#include "main.h"

Display::Display(float x, float y, color_t color, int mask) {
    this->position = glm::vec3(x, y, 0);
    this->mask = mask;
    const float side = 0.1, thickness = 0.03;
    GLfloat vertex_buffer_data_display_hor[] = {
        thickness, 0, 0,
        thickness, -thickness, 0,
        side+thickness,  -thickness, 0,
        side+thickness, 0, 0,
    };
    for(int i=0; i<3; ++i) {
        this->object[i] = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_display_hor, color, GL_FILL);
        for(int j=1; j<12; j+=3) {
            vertex_buffer_data_display_hor[j] -= side + thickness;
        }
    }
    GLfloat vertex_buffer_data_display_ver[] = {
        0, -thickness, 0,
        0, -thickness - side, 0,
        thickness,  -thickness - side, 0,
        thickness, -thickness, 0,
    };
    for(int i=3; i<7; ++i) {
        int diff = i - 3;
        // X - Co-ordinates
        for(int j=0; j<12; j+=3) {
            if(diff & 1) vertex_buffer_data_display_ver[j] += side + thickness;
        }
        // Y - Co-ordinates
        for(int j=1; j<12; j+=3) {
            if(diff & 2) vertex_buffer_data_display_ver[j] -= side + thickness;
        }
        this->object[i] = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_display_ver, color, GL_FILL);
        // X - Co-ordinates
        for(int j=0; j<12; j+=3) {
            if(diff & 1) vertex_buffer_data_display_ver[j] -= side + thickness;
        }
        // Y - Co-ordinates
        for(int j=1; j<12; j+=3) {
            if(diff & 2) vertex_buffer_data_display_ver[j] += side + thickness;
        }
    }
    GLfloat vertex_buffer_data_display_diag1[] = {
        0, -thickness, 0,
        thickness, 0, 0,
        2*thickness + side, -2*thickness - 2*side, 0,
        thickness + side, -3*thickness - 2*side, 0,
    };
    this->object[7] = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_display_diag1, color, GL_FILL);
    GLfloat vertex_buffer_data_display_diag2[] = {
        2*thickness + side, -thickness, 0,
        thickness + side, 0, 0,
        0, -2*thickness - 2*side, 0,
        thickness, -3*thickness - 2*side, 0,
    };
    this->object[8] = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_display_diag2, color, GL_FILL);
}

void Display::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (0.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(int i=0; i<sizeof(this->object)/sizeof(this->object[0]); ++i) {
        if(mask & (1<<i)) draw3DObject(this->object[i]);
    }
}