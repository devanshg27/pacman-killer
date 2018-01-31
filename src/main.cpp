#include "main.h"
#include "timer.h"
#include "ball.h"
#include "physics.h"
#include "ground.h"
#include "pool.h"
#include "trampoline.h"
#include "porcupine.h"
#include "magnet.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball ball2;
Ground ground1, ground2;
Pool pool1;
Trampoline trampoline1;
Porcupine porcupine1;
Magnet magnet1;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float magnetForce = 1;

Timer t60(1.0 / 60);
const float PI = acos(-1);

void inputHandler(int key, int action) {
    if(key == GLFW_KEY_LEFT) {
        if(action == GLFW_PRESS) {
            ball2.velocity.x -= 2.5f;
            if(ball2.velocity.x < -4.0f) ball2.velocity.x = min(ball2.velocity.x + 2.5f, -4.0f);
            ball2.acceleration.x -= 1.5f;
        }
        else if(action == GLFW_RELEASE) {
            if(ball2.velocity.x < 0) ball2.velocity.x = 0;
            ball2.acceleration.x += 1.5f;
        }
    }
    if(key == GLFW_KEY_RIGHT) {
        if(action == GLFW_PRESS) {
            ball2.velocity.x += 2.5f;
            if(ball2.velocity.x > 4.0f) ball2.velocity.x = max(ball2.velocity.x - 2.5f, 4.0f);
            ball2.acceleration.x += 1.5f;
        }
        else if(action == GLFW_RELEASE) {
            ball2.acceleration.x -= 1.5f;
            if(ball2.velocity.x > 0) ball2.velocity.x = 0;
        }
    }
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    trampoline1.draw(VP);
    ground1.draw(VP);
    ground2.draw(VP);
    pool1.draw(VP);
    porcupine1.draw(VP);
    magnet1.draw(VP);
    ball2.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    if(detect_collision(ball2.shape, pool1.shape).first.first) {
        if(up) {
            ball2.velocity.y = 3.5;
        }
        ball2.handleCollision(detect_collision(ball2.shape, pool1.shape).second, pool1.restitution, detect_collision(ball2.shape, pool1.shape).first.second);
    }
    if(detect_collision(ball2.shape, ground1.shape).first.first) {
        if(up) {
            ball2.velocity.y = 4.5;
        }
        ball2.handleCollision(detect_collision(ball2.shape, ground1.shape).second, ground1.restitution, detect_collision(ball2.shape, ground1.shape).first.second);
    }
    if(detect_collision(ball2.shape, ground2.shape).first.first) {
        if(up) {
            ball2.velocity.y = 4.5;
        }
        ball2.handleCollision(detect_collision(ball2.shape, ground2.shape).second, ground2.restitution, detect_collision(ball2.shape, ground2.shape).first.second);
    }
    if(detect_collision(ball2.shape, trampoline1.shape).first.first and ball2.velocity.y < 0) {
        ball2.handleCollision(detect_collision(ball2.shape, trampoline1.shape).second, trampoline1.restitution, detect_collision(ball2.shape, trampoline1.shape).first.second);
    }
    if(detect_collision(ball2.shape, porcupine1.shape).first.first) {
        printf("Spiked\n");
        ball2.handleCollision(detect_collision(ball2.shape, porcupine1.shape).second, porcupine1.restitution, detect_collision(ball2.shape, porcupine1.shape).first.second);
    }
}

void tick_elements() {
    ball2.tick(1.0/60, ball2.shape.centerY - ball2.shape.radius < -2.51 );
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball2       = Ball(1, -2, -1.5+ball2.bounding_box().height/2, COLOR_RED);
    ground1     = Ground(-4, -3.5, COLOR_GREEN, COLOR_BROWN);
    ground2     = Ground(4, -3.5, COLOR_GREEN, COLOR_BROWN);
    pool1     = Pool(0, -3.5, COLOR_GREEN, COLOR_BROWN, COLOR_BLUE);
    trampoline1     = Trampoline(4, -2.5, COLOR_RED);
    porcupine1     = Porcupine(-4, -2.5, COLOR_RED);
    magnet1     = Magnet(0, 0, COLOR_BROWN, COLOR_BLUE, PI);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1600;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_input(window);
            tick_elements();
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left*16.0f/9, right*16.0f/9, bottom, top, 0.1f, 500.0f);
}
