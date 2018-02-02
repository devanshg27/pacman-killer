#include <vector>
#include <algorithm>
#include "main.h"
#include "timer.h"
#include "ball.h"
#include "physics.h"
#include "ground.h"
#include "pool.h"
#include "trampoline.h"
#include "porcupine.h"
#include "magnet.h"
#include "enemy.h"
#include "display.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

vector<pair<int, int>> levelInfo;
glm::mat4 initVP;
vector<Ground> groundList;
vector<Pool> poolList;
vector<Trampoline> trampolineList;
vector<Porcupine> porcupineList;
vector<Display> displayList;
vector<Enemy> enemyList;
int groundWidth = 5;
int level, score, lives, timeLeft;
int poolWidth = 3, leftBorder, rightBorder;
int maskArr[256];
Ball ball2;
Magnet magnet1;
char windowTitle[256];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float magnetForce;
bool lbutton_down = false;
float previous_x_position;
int curMouseVel = 0;

Timer t60(1.0 / 60);
const float PI = acos(-1);

void inputHandler(int key, int action) {
    if(lbutton_down) return;
    if(key == GLFW_KEY_A) {
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
    if(key == GLFW_KEY_D) {
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
    for(auto&z: trampolineList) {
        z.draw(VP);
    }
    for(auto&z: groundList) {
        z.draw(VP);
    }
    for(auto&z: poolList) {
        z.draw(VP);
    }
    for(auto&z: porcupineList) {
        if(z.isAlive) z.draw(VP);
    }
    for(auto&z: enemyList) {
        z.draw(VP);
    }
    for(auto&z: displayList) {
        z.draw(initVP);
    }
    if(fabs(magnetForce) > 0.1) magnet1.draw(VP);
    ball2.draw(VP);
}

void tick_input(GLFWwindow *window) {
    if(lbutton_down and curMouseVel == 0) {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);
        if(x > previous_x_position) {
            ball2.velocity.x += 2.5f;
            if(ball2.velocity.x > 4.0f) ball2.velocity.x = max(ball2.velocity.x - 2.5f, 4.0f);
            ball2.acceleration.x += 1.5f;
            curMouseVel = 1;
        }
        else if(x < previous_x_position){
            ball2.velocity.x -= 2.5f;
            if(ball2.velocity.x < -4.0f) ball2.velocity.x = min(ball2.velocity.x + 2.5f, -4.0f);
            ball2.acceleration.x -= 1.5f;
            curMouseVel = -1;
        }
        previous_x_position = x;
    }
    else if(not lbutton_down and curMouseVel == 1){
        ball2.acceleration.x -= 1.5f;
        if(ball2.velocity.x > 0) ball2.velocity.x = 0;
        curMouseVel = 0;
    }
    else if(not lbutton_down and curMouseVel == -1){
        if(ball2.velocity.x < 0) ball2.velocity.x = 0;
        ball2.acceleration.x += 1.5f;
        curMouseVel = 0;
    }

    int up = glfwGetKey(window, GLFW_KEY_SPACE);
    if(glfwGetKey(window, GLFW_KEY_LEFT)) {
        screen_center_x -= 0.01f;
        reset_screen();
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT)) {
        screen_center_x += 0.01f;
        reset_screen();
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN)) {
        screen_center_y -= 0.01f;
        reset_screen();
    }
    if(glfwGetKey(window, GLFW_KEY_UP)) {
        screen_center_y += 0.01f;
        reset_screen();
    }

    enemyList.erase(std::remove_if(enemyList.begin(), enemyList.end(), [](const Enemy &en) {
        return fabs(en.position.x - ball2.position.x) > 40;
    }), enemyList.end());

    for(auto&z: poolList) {
        auto val = detect_collision(ball2.shape, z.shape);
        if(val.first.first) {
            if(up) {
                ball2.velocity.y = 3.5;
            }
            ball2.handleCollision(val.second, z.restitution, val.first.second);
        }
    }
    for(auto&z: groundList) {
        auto val = detect_collision(ball2.shape, z.shape);
        if(val.first.first) {
            if(up) {
                ball2.velocity.y = 4.5;
            }
            ball2.handleCollision(val.second, z.restitution, val.first.second);
        }
    }
    for(auto&z: trampolineList) {
        auto val = detect_collision(ball2.shape, z.shape);
        if(val.first.first and ball2.velocity.y < 0.001) {
            ball2.handleCollision(val.second, z.restitution, val.first.second);
        }
    }
    for(auto&z: porcupineList) {
        if(not z.isAlive) continue;
        auto val = detect_collision(ball2.shape, z.shape);
        if(val.first.first) {
            z.isAlive = false;
            --lives;
            ball2.handleCollision(val.second, z.restitution, val.first.second);
        }
    }
    for(auto it = enemyList.begin(); it != enemyList.end(); ) {
        auto val = detect_collision(ball2.shape, it->shape);
        if(val.first.first) {
            score += 50;
            ball2.handleCollision(val.second, it->restitution, val.first.second);
            it = enemyList.erase(it);
            continue;
        }
        val = detect_collision(ball2.shape, it->enemyBall);
        if(val.first.first and ball2.velocity.y < -0.02) {
            score += 100;
            ball2.velocity.y = 4.2;
            it = enemyList.erase(it);
            continue;
        }
        ++it;
    }
}

void addWorld() {
    // 1 = normal ground
    // 2 = medium high ground
    // 3 = higher ground
    // 4 = trampoline ground
    // 5 = pool ground
    // 6 = porcupine ground

    while(rightBorder <= ball2.position.x + 40) {
        int temp = 1 + rand() % 6;
        if(temp == 1) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            rightBorder += groundWidth;
        }
        else if(temp == 2) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3, COLOR_GREEN, COLOR_BROWN));
            rightBorder += groundWidth;
        }
        else if(temp == 3) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -2.25, COLOR_GREEN, COLOR_BROWN));
            rightBorder += groundWidth;
        }
        else if(temp == 4) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            trampolineList.push_back(Trampoline(rightBorder + groundWidth/2.0, -2.5, COLOR_BLACK));
            rightBorder += groundWidth;
        }
        else if(temp == 5) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            rightBorder += groundWidth;
            poolList.push_back(Pool(rightBorder + poolWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN, COLOR_BLUE));
            rightBorder += poolWidth;
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            rightBorder += groundWidth;
        }
        else if(temp == 6) {
            groundList.push_back(Ground(rightBorder + groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            porcupineList.push_back(Porcupine(rightBorder + groundWidth/2.0, -2.5, COLOR_PORCUPINE, 0.01, rightBorder + groundWidth/2.0 - 2, rightBorder + groundWidth/2.0 + 0.5));
            porcupineList.back().isAlive = (level >= 3);
            rightBorder += groundWidth;
        }
    }

    while(leftBorder >= ball2.position.x - 40) {
        int temp = 1 + rand() % 6;
        if(temp == 1) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            leftBorder -= groundWidth;
        }
        else if(temp == 2) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3, COLOR_GREEN, COLOR_BROWN));
            leftBorder -= groundWidth;
        }
        else if(temp == 3) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -2.25, COLOR_GREEN, COLOR_BROWN));
            leftBorder -= groundWidth;
        }
        else if(temp == 4) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            trampolineList.push_back(Trampoline(leftBorder - groundWidth/2.0, -2.5, COLOR_BLACK));
            leftBorder -= groundWidth;
        }
        else if(temp == 5) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            leftBorder -= groundWidth;
            poolList.push_back(Pool(leftBorder - poolWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN, COLOR_BLUE));
            leftBorder -= poolWidth;
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            leftBorder -= groundWidth;
        }
        else if(temp == 6) {
            groundList.push_back(Ground(leftBorder - groundWidth/2.0, -3.5, COLOR_GREEN, COLOR_BROWN));
            porcupineList.push_back(Porcupine(leftBorder - groundWidth/2.0, -2.5, COLOR_PORCUPINE, 0.01, leftBorder - groundWidth/2.0 - 2, leftBorder - groundWidth/2.0 + 0.5));
            porcupineList.back().isAlive = (level >= 3);
            leftBorder -= groundWidth;
        }
    }
}

void addEnemy() {
    float xPos = (16.0 * (screen_center_x - 4 / screen_zoom) / 9) - 0.5;
    float yPos = -1 + 5*(rand()/(double)RAND_MAX);
    enemyList.push_back(Enemy(xPos, yPos, COLOR_PLANK, COLOR_BALL, 0.01 + 0.01*(rand()/(double)RAND_MAX), ((-75 + 150.0 * (rand()/(double)RAND_MAX)) * PI / 180.0), bool(rand() & 1)));
}

void tick_elements() {
    ball2.tick(1.0/60, ball2.shape.centerY - ball2.shape.radius < -2.51 );
    for(auto&z: porcupineList) {
        if(z.isAlive) z.move();
    }
    for(auto&z: enemyList) {
        z.move();
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    ball2 = Ball(0, -1.5, COLOR_RED);

    magnet1 = Magnet(0, 0, COLOR_RED, COLOR_BLACK, PI);
    magnetForce = 0;

    levelInfo = {{2000, 80}, {4000, 60}, {6000, 60}, {9000, 80}, {13000, 100}};

    initVP = glm::ortho(-4*16.0f/9, 4*16.0f/9, -4.0f, 4.0f, 0.1f, 500.0f) * glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    lives = 3;
    level = 1;
    score = 0;
    timeLeft = 60 * levelInfo[level-1].second;

    groundList.push_back(Ground(-4, -3.5, COLOR_GREEN, COLOR_BROWN));
    groundList.push_back(Ground(4, -3.5, COLOR_GREEN, COLOR_BROWN));
    rightBorder = groundWidth + poolWidth/2.0;
    leftBorder = -rightBorder;

    poolList.push_back(Pool(0, -3.5, COLOR_GREEN, COLOR_BROWN, COLOR_BLUE));

    trampolineList.push_back(Trampoline(4, -2.5, COLOR_BLACK));

    porcupineList.push_back(Porcupine(-4, -2.5, COLOR_PORCUPINE, 0.01, -6, -3.5));

    for(int i=0; i<100; ++i) {
        displayList.push_back(Display((-4 * 16.0/9) + 0.1 + i*0.2, 3.8, COLOR_BLACK, maskArr[' ']));
    }

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

void initMasks() {
    maskArr[' '] = 0;
    maskArr['-'] = 0b10;
    maskArr['L'] = 0b101100;
    maskArr['V'] = 0b11010000;
    maskArr['E'] = 0b101111;
    maskArr['C'] = 0b101101;
    maskArr['T'] = 0b101001;
    maskArr['F'] = 0b101011;
    maskArr['N'] = 0b11111000;
    maskArr['m'] = 0b010101000;
    maskArr['M'] = 0b101010000;
    maskArr['R'] = 0b1111011;
    maskArr['X'] = 0b110000000;
    maskArr['0'] = maskArr['O'] = 0b1111101;
    maskArr['I'] = maskArr['1'] = 0b1010000;
    maskArr['2'] = 0b0110111;
    maskArr['3'] = 0b1010111;
    maskArr['4'] = 0b1011010;
    maskArr['S'] = maskArr['5'] = 0b1001111;
    maskArr['6'] = 0b1101111;
    maskArr['7'] = 0b1010001;
    maskArr['8'] = 0b1111111;
    maskArr['9'] = 0b1011111;
}

void nextLevel() {
    if(score >= levelInfo[level-1].first) {
        ball2 = Ball(0, -1.5, COLOR_RED);
        enemyList.clear();
        magnetForce = 0;
        screen_center_x = 0;
        screen_center_y = 0;
        curMouseVel = 0;

        ++level;
        if(level >= 3) {
            for(auto&z: porcupineList) {
                z.isAlive = true;
            }
        }
        timeLeft = 60 * levelInfo[level-1].second;
    }
}

int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1600;
    int height = 900, cnt = 0, nxtEnemyCnt = 0, nxtMagnetCnt = 2000/60, nxtPorcupineCnt = 10000/60;

    window = initGLFW(width, height);
    initMasks();
    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        if(level > levelInfo.size()) {
            printf("You completed the Game. Your score was %d\n", score);
            exit(0);
        }
        if(lives < 0 or timeLeft < 0) {
            printf("You lost the Game. Your score was %d\n", score);
            exit(0);
        }
        if (t60.processTick()) {
            // 60 fps
            --timeLeft;
            // OpenGL Draw commands
            reset_screen();
            addWorld();
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            sprintf(windowTitle, "Pacman Killer, Level: %d, Score: %d, Lives Left: %d, Time Left: %d, Next Level: %d", level, score, lives, (timeLeft+59)/60, levelInfo[level-1].first);
            glfwSetWindowTitle(window, windowTitle);
            sprintf(windowTitle, "LEVEL-%d  SCORE-%d  LIVES LEFT-%d  TImME LEFT- %d  NEXT LEVEL-%d", level, score, lives, (timeLeft+59)/60, levelInfo[level-1].first);

            for(int i=0; i<displayList.size(); ++i) {
                displayList[i].mask = maskArr[' '];
            }

            for(int i=0; windowTitle[i]!='\0'; ++i) {
                displayList[i].mask = maskArr[windowTitle[i]];
            }

            tick_input(window);
            tick_elements();
            ++cnt;
            if(cnt >= nxtEnemyCnt) {
                nxtEnemyCnt = 30 + cnt + (rand() & 63);
                addEnemy();
            }
            if(level >=2 and cnt >= nxtMagnetCnt) {
                if(fabs(magnetForce) > 0.1) {
                    nxtMagnetCnt = 7000/60 + cnt + (rand() & 255);
                    magnetForce = 0;
                }
                else {
                    nxtMagnetCnt = 22000/60 + cnt + (rand() & 255);
                    int temp = rand() % 2;
                    magnetForce = 2*temp - 1;
                    magnet1.rotation = temp * PI;
                }
            }
            if(cnt >= nxtPorcupineCnt and level >= 3) {
                nxtPorcupineCnt = 10000/60 + cnt + (rand() & 1023);
                for(auto&z: porcupineList) {
                    int temp = rand() % 2;
                    if(not z.isAlive and temp) z.isAlive = true;
                }
            }
            nextLevel();
        }
        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;

    left *= 16.0f/9;
    right *= 16.0f/9;

    float edgeRatio = 0.85;

    float temp = edgeRatio * top + (1 - edgeRatio) * bottom;
    if(ball2.position.y > temp) {
        float diff = ball2.position.y - temp;
        screen_center_y += diff;
        top += diff;
        bottom += diff;
    }
    temp = edgeRatio * bottom + (1 - edgeRatio) * top;
    if(ball2.position.y < temp) {
        float diff = ball2.position.y - temp;
        screen_center_y += diff;
        top += diff;
        bottom += diff;
    }
    temp = edgeRatio * right + (1 - edgeRatio) * left;
    if(ball2.position.x > temp) {
        float diff = ball2.position.x - temp;
        screen_center_x += diff;
        right += diff;
        left += diff;
    }
    temp = edgeRatio * left + (1 - edgeRatio) * right;
    if(ball2.position.x < temp) {
        float diff = ball2.position.x - temp;
        screen_center_x += diff;
        right += diff;
        left += diff;
    }
    if(magnet1.rotation == 0) magnet1.set_position(left - 0.42, (top*(0.8) + bottom*(1-0.8)));
    else magnet1.set_position(right + 0.42, (top*0.8 + bottom*(1-0.8)));
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
