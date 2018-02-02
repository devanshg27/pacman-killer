#include "enemy.h"
#include "main.h"

Enemy::Enemy(float x, float y, color_t colorPlank, color_t colorBall, float speed, float rotation, bool hasPlank) {
    this->position = glm::vec3(x, y, 0);
    this->restitution = 1;
    this->rotation = rotation;
    this->speed = speed;

    const int sides = 50;
    float width = 0.09, cosAng = cos(rotation), sinAng = sin(rotation);
    GLfloat vertex_buffer_data_ball[6 + 3*sides]; // Triangle fan requires N+2 vertices for N triangles

    vertex_buffer_data_ball[0] = 0.0f;
    vertex_buffer_data_ball[1] = 0.0f;
    vertex_buffer_data_ball[2] = 0.0f;
    for(int i=0; i<=sides; ++i) {
        vertex_buffer_data_ball[3+3*i] = 0.2f*cos(i*2*PI/sides);
        vertex_buffer_data_ball[4+3*i] = 0.2f*sin(i*2*PI/sides);
        vertex_buffer_data_ball[5+3*i] = 0.0f;
    }

    const float side = 0.27;

    GLfloat vertex_buffer_data_plank[] = {
        -side, 0.26, 0,
        -side, 0.2, 0,
        side,  0.2, 0,
        side, 0.26, 0,
    };

    if(not hasPlank) {
        for(int i=1; i<=10; i+=3) {
            vertex_buffer_data_plank[i] *= 1000;
        }
        this->shape = Rectangle(x-side, y+200, x+side, y+230, 0);
    }
    else {
        float tempX = x - 0.2 * sinAng - (side * cosAng);
        float tempY = y + 0.2 * cosAng - (side * sinAng);
        this->shape = Rectangle(tempX, tempY, tempX+side*2, tempY+0.06, this->rotation * 180.0 / PI);
    }
    this->plankObject = create3DObject(GL_TRIANGLE_FAN, 4, vertex_buffer_data_plank, colorPlank, GL_FILL);

    this->ballObject = create3DObject(GL_TRIANGLE_FAN, sizeof(vertex_buffer_data_ball)/(sizeof(vertex_buffer_data_ball[0])*3), vertex_buffer_data_ball, colorBall, GL_FILL);
    this->enemyBall = Rectangle(x-width, y, x+width, y+sqrt(0.2*0.2 - width*width), 0);
}

void Enemy::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->ballObject);
    draw3DObject(this->plankObject);
}

void Enemy::move() {
    this->position = glm::vec3(this->position.x + this->speed, this->position.y, 0);
    this->shape.xLeft += this->speed;
    this->shape.xRight += this->speed;
    this->enemyBall.xLeft += this->speed;
    this->enemyBall.xRight += this->speed;
}
