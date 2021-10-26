#include <iostream>
#include <string>
#include <stdlib.h>
#define GLEW_STATIC
#include "loadimage.h"
#include "drawing.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

GLFWwindow* window = NULL;
const char* WINDOW_TITLE = "Coursework";
const GLint WINDOW_WIDTH = 800;
const GLint WINDOW_HEIGHT = 600;
bool wKey, sKey, aKey, dKey, torch;
bool initOpenGL();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void downLight(int state);
int light_state(int frame);
glm::vec3 position_manager(glm::vec3 pos);
glm::vec4 generateStarfield(float minDistance, float maxDistance);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
  if (!initOpenGL())
    {
        std::cerr << "GLFW initialisation failed." << std::endl;
        return -1;
    }
    
    /*Tracking Variables*/
    const double fpsLimit = 1.0 / 60.0;
    double lastFrameTime = 0;
    int frame = 0;
    int door_state = 0;
    float worldRot = 0.f;
    int starCount = 300;
    glm::vec4 starPositions[starCount];
    float starMinDistance = 30.f;
    float starMaxDistance = 49.f;
    int i;
    for (i=0; i < starCount; i++)
    {
        starPositions[i] = generateStarfield(starMinDistance, starMaxDistance);
    }
    
    /*OpenGL Settings*/
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    
    /*Torch Light Settings*/
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float ambient_l[] = { 0.8f, 0.8f, 0.7f, 1.0f };
    float diffuse_l[] = { 0.8f, .8f, 0.7f, 1.0f };
    float specular_l[] = { 0.9f, 0.9f, 0.7f, 1.0f };
    GLfloat spot_dir[] = { 0.f, 0.f, -1.f };
    GLfloat spot_cutoff = 20.0f;
    GLfloat spot_exponent = 60.f;
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_l);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_l);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_l);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
    
    /*Initialisation of all textures*/
    GLuint floor = loadimage("floor.bmp");
    GLuint screenA;
    GLuint screenB;
    GLuint button;
    GLuint door;
    GLuint screen1a = loadimage("screen1a.bmp");
    GLuint screen1b = loadimage("screen1b.bmp");
    GLuint screen1c = loadimage("screen1c.bmp");
    GLuint screen2a = loadimage("screen2a.bmp");
    GLuint screen2b = loadimage("screen2b.bmp");
    GLuint screen2c = loadimage("screen2c.bmp");
    GLuint button1 = loadimage("button1.bmp");
    GLuint button2 = loadimage("button2.bmp");
    GLuint button3 = loadimage("button3.bmp");
    GLuint door_open = loadimage("door_open.bmp");
    GLuint door_closed = loadimage("door_closed.bmp");
    GLuint sky = loadimage("sky.bmp");
   
    /*Camera Initialisation*/
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    float sensitivity = 0.02f;
    float cameraSpeed = 0.04f;
    glm::vec3 pos = glm::vec3(0.f, 3.0f, -4.0f);
    glm::vec3 viewDir = glm::vec3(0.5f, -0.3f, 1.0f);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    /*MAIN LOOP*/
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, WINDOW_WIDTH*2, WINDOW_HEIGHT*2); // *2 neccessary for Mac
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspectRatio = static_cast<GLdouble>(WINDOW_WIDTH) / static_cast<GLdouble>(WINDOW_HEIGHT);
        float znear = 0.1f;
        float zfar = 1000.f;
        float fovyInDegrees = 80.f;
        glm::mat4 proj = glm::perspective(glm::radians(fovyInDegrees), aspectRatio, znear, zfar);
        glLoadMatrixf(glm::value_ptr(proj));
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float position_0[] = { 0.f, 0.f, 0.f, 1.f };
        glLightfv(GL_LIGHT0, GL_POSITION, position_0);
        
        /* Camera Update */
        glm::vec3 eye = pos;
        glm::vec3 cen = pos + viewDir;
        glm::vec3 cameraDirection = glm::normalize(cen - eye);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
        glm::mat4 view = glm::mat4(
            cameraRight[0], cameraUp[0], -cameraDirection[0], 0.0f,
            cameraRight[1], cameraUp[1], -cameraDirection[1], 0.0f,
            cameraRight[2], cameraUp[2], -cameraDirection[2], 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        );
        view = glm::translate(view, -eye);
        glLoadMatrixf(glm::value_ptr(view));

        // Camera Controls
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        viewDir = viewDir + cameraRight * (float)((xpos - mouseX) * sensitivity);
        viewDir = viewDir + cameraUp * (float)(-(ypos - mouseY) * sensitivity);
        mouseX = xpos;
        mouseY = ypos;

        // Key input
        if (wKey)
        {
            pos = pos + cameraDirection * cameraSpeed;
        }

        if (sKey)
        {
            pos = pos + cameraDirection * -cameraSpeed;
        }

        if (aKey)
        {
            pos = pos + cameraRight * -cameraSpeed;
        }

        if (dKey)
        {
            pos = pos + cameraRight * cameraSpeed;
        }
        if (torch){
            glEnable(GL_LIGHT0);
        }
        if (!torch){
            glDisable(GL_LIGHT0);
        }
        
        /* World State Managers */
        int l_state = light_state(frame);
        worldRot += 0.05f;
        pos = position_manager(pos);
        
        if (pos[2] > -10.f && pos[2] < -6.f)  //Door Activation
        {
            door_state = 1;
            door = door_open;
        }
        else {
            door_state = 0;
            door = door_closed;
        }

        /* Screen State Manager */
        int screen_position = frame % 300;
        if (screen_position < 150) {
            screenA = screen1a;
            screenB = screen2a;
            button = button3;
        }
        else if (screen_position >= 150 && screen_position < 200) {
            button = button1;
        }
        else if (screen_position >= 200 && screen_position < 210) {
            screenA = screen1b;
        }
        else if (screen_position >= 210 && screen_position < 220) {
            screenA = screen1a;
            screenB = screen2b;
            button = button2;
        }
        else if (screen_position >= 220 && screen_position < 230) {
            screenB = screen1b;
        }
        else if (screen_position >= 230 && screen_position < 300) {
            screenA = screen1c;
            screenB = screen2c;
            button = button1;
        }
        
        glPushMatrix(); ///SKY
            glRotatef(worldRot, .3f, 0.3f, 0.2f);
            glRotatef(worldRot * 0.3, .7f, 0.1f, 0.5f);
            glPushMatrix();
                glTranslatef(10.f, 30.f, 30.f);
                glScalef(5.f, 5.f, 5.f);
                drawSphere(1.f, 4, 7);
                float ambient_x[] = { 0.f, 0.f, 0.f, 1.0f };
                float diffuse_x[] = { .5f, .25f, 0.1f, 1.0f };
                float specular_x[] = { .5f, .25f, 0.1f, 1.0f };
                glLightfv(GL_LIGHT7, GL_AMBIENT, ambient_x);
                glLightfv(GL_LIGHT7, GL_DIFFUSE, diffuse_x);
                glLightfv(GL_LIGHT7, GL_SPECULAR, specular_x);
                glEnable(GL_LIGHT7);
                float position_x[] = { 0.0f, 0.0f, 0.f, 1.f };
                glLightfv(GL_LIGHT7, GL_POSITION, position_x);
            glPopMatrix();
            glPushMatrix();
                int j;
                for (j=0; j < starCount; j++)
                {
                    drawStarfield(starPositions[j], pos);
                }
            glPopMatrix();
            glScalef(50.f, 50.f, 50.f);
            drawSkydome(1.f, 3, sky);
        glPopMatrix();
        
        glPushMatrix(); ///SHIP
            glPushMatrix();
                downLight(l_state);
            glPopMatrix();
            
        glPushMatrix();  ///FLOOR
                glTranslatef(0.0f, -0.5f, 0.0f);
                drawFloor(4.0f, 0.05f, 4.0f, 6, floor, 10);
            glPopMatrix();
                
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, .5f);
                    drawCylinder(0.5f, 0.1f, 40, 20, 0.0f, -0.5f, 0.0f, 0);
                    drawCylinder(0.1f, 0.5f, 40, 20, 0.0f, -0.4f, 0.0f, 0);
                glPushMatrix();
                    glTranslatef(0.0f, 0.1f, 0.0f);
                    glRotatef(-30, 0.0f, 1.0f, 0.0f);
                    drawSolidCube(0.5f, 0.1f, 0.5f, 4, 5); // Chair Seat
                    glPushMatrix();
                        glTranslatef(0.0f, 0.3f, -0.6f);
                        drawSolidCube(0.5f, 0.4f, 0.1f, 4, 5); // Chair Back
                        glRotatef(90, 0.f, 0.f, 1.f);
                        glTranslatef(.4f, -0.5f, 0.f);
                        drawCylinder(0.12f, 0.99f, 10, 5, 0.0f, 0.0f, 0.0f, 5);
                    glPopMatrix();
                glPopMatrix();
                
                glPushMatrix(); //Right Control Panel
                    glTranslatef(1.5f, 0.f, 0.f);
                    glRotatef(-10, 0.f, 1.f, 0.f);
                    drawSolidCube(0.3f, 0.5f, 1.0f, 4, 6);
                    glPushMatrix();
                        glTranslatef(0.f, -0.501f, -0.95f);
                        drawCylinder(.3f, 1.f, 10, 5, 0.f, 0.f, 0.f, 0);
                    glPopMatrix();
                    glTranslatef(0.f, .65f, 0.f);
                    glRotatef(90, 0.f, 1.f, 0.f);
                    drawWedge(1.f, 0.15f, 0.3f, 4, 0);
                        //Transformations for screen placement
                        glTranslatef(-.6f, -0.14f, 0.15f);
                        glScalef(0.22f, .22f, .22f);
                        glRotatef(-26, 1.0f, 0.0f, 0.0f);
                        drawScreen(1.0f, 1.0f, 1.0f, screenA);
                        glTranslatef(2.f, 0.f, 0.f);
                        drawScreen(1.f, 1.0f, 1.0f, screenA);
                        glTranslatef(3.f, 0.f, 0.f);
                        drawScreen(1.5f, 1.0f, 1.0f, screenB);
                glPopMatrix();
            
                ///LEFT WALL PANEL
                glPushMatrix();
                    glTranslatef(-3.7f, 0.f, -2.1f);
                    glRotatef(180, 0.f, 1.f, 0.f);
                    drawSolidCube(0.3f, 0.5f, 1.5f, 4, 6);
                    glTranslatef(0.f, .65f, 0.f);
                    glRotatef(90, 0.f, 1.f, 0.f);
                    drawWedge(1.5f, 0.15f, 0.3f, 4, 0);
                    glTranslatef(-1.f, -0.13f, 0.17f);
                    glScalef(0.2f, .2f, .2f);
                    glRotatef(-26, 1.0f, 0.0f, 0.0f);
                    drawScreen(1.0f, 1.0f, 1.0f, screenA);
                    glTranslatef(3.f, 0.f, 0.f);
                    drawScreen(1.0f, 1.0f, 1.0f, screenA);
                    glTranslatef(4.f, 0.f, 0.f);
                    drawScreen(2.0f, 1.0f, 1.0f, screenB);
                glPopMatrix();
        
                ///LEFT  MID WALL
                glPushMatrix();
                    glTranslatef(-4.1f, 0.f, -1.6f);
                    glPushMatrix();
                        drawSolidCube(0.1f, 1.f, 2.f, 4, 2);  //Lower Left Wall
                        glTranslatef(0.f, 4.f, 0.f);
                        drawSolidCube(0.1f, .3f, 2.f, 4, 2);  //Upper Left wall
                        glPushMatrix();
                            glTranslatef(1.f, 0.f, 0.f);
                            drawSolidCube(1.f, 0.1f, 2.f, 4, 2);  //Roof Left
                            glTranslatef(3.f, 0.f, 0.f);
                            drawSolidCube(2.2f, 0.05f, 2.f, 5, 3);  //Roof Window
                        glPopMatrix();
                        glTranslatef(0.f, -3.7f, 2.1f);
                        drawSolidCube(0.1f, 4.f, .1f, 4, 2);  //Window cap
                        glTranslatef(0.f, 1.5f, -2.2f);
                        drawSolidCube(0.05f, 2.f, 2.2f, 5, 3);  //Window
                    glPopMatrix();
                    ///LEFT FRONT WALL
                    glPushMatrix();
                        glTranslatef(0.2f, 0.f, 2.3f);
                        glPushMatrix();
                            glRotatef(-90, 0.f, 0.f, 1.f);
                            drawWedge(1.f, 0.2f, .2f, 4, 2);
                        glPopMatrix();
                        glRotatef(-45, 0.f, 1.f, 0.f);
                        glTranslatef(1.1f, 0.f, 0.1f);
                        drawSolidCube(1.f, 1.f, 0.101f, 4, 2);
                        glTranslatef(-0.2f, 2.f, 0.f);
                        drawSolidCube(1.2f, 2.f, 0.05f, 4, 3);
                        glPushMatrix();
                            glTranslatef(0.f, 2.f, 0.f);
                            glRotatef(-90, 0.f, 0.f, 1.f);
                            glRotatef(135, 1.f, 0.f, 0.f);
                            drawWedge(0.1f, 1.f, 1.f, 4, 2);
                        glPopMatrix();
                        glTranslatef(1.3f, -0.3f, 0.f);
                        drawSolidCube(0.1f, 2.3f, 0.1f, 4, 2);
                    glPopMatrix();
                glPopMatrix();
        
            ///RIGHT MID WALL
            glPushMatrix();
                glTranslatef(4.1f, 0.f, -1.6f);
                glPushMatrix();
                    drawSolidCube(0.1f, 1.f, 2.f, 4, 2);  //Lower Right Wall
                    glTranslatef(0.f, 4.f, 0.f);
                    drawSolidCube(0.1f, .3f, 2.f, 4, 2);  //Upper Right wall
                    glPushMatrix();
                        glTranslatef(-1.f, 0.f, 0.f);
                        drawSolidCube(1.f, 0.1f, 2.f, 4, 2);  //Roof Right
                    glPopMatrix();
                    glTranslatef(0.f, -3.7f, 2.1f);
                    drawSolidCube(0.1f, 4.f, .1f, 4, 2);  //Window cap
                    glTranslatef(0.f, 1.5f, -2.2f);
                    drawSolidCube(0.05f, 2.f, 2.f, 5, 3);  //Window Right
                glPopMatrix();
                ///FRONT RIGHT WALL
                glPushMatrix();
                    glTranslatef(-0.2f, 0.f, 2.3f);
                    glPushMatrix();
                        glRotatef(90, 0.f, 0.f, 1.f);
                        drawWedge(1.f, 0.2f, .2f, 4, 2);
                    glPopMatrix();
                    glRotatef(45, 0.f, 1.f, 0.f);
                    glTranslatef(-1.1f, 0.f, 0.1f);
                    drawSolidCube(1.f, 1.f, 0.101f, 4, 2);
                    glTranslatef(0.2f, 2.f, 0.f);
                    drawSolidCube(1.2f, 2.f, 0.05f, 4, 3);
                    glPushMatrix();
                        glTranslatef(0.f, 2.f, 0.f);
                        glRotatef(90, 0.f, 0.f, 1.f);
                        glRotatef(135, 1.f, 0.f, 0.f);
                        drawWedge(0.1f, 1.f, 1.f, 4, 2);
                    glPopMatrix();
                    glTranslatef(-1.3f, -0.3f, 0.f);
                    drawSolidCube(0.1f, 2.3f, 0.1f, 4, 2);
                glPopMatrix();
            glPopMatrix();
        
            ///RIGHT WALL
            glPushMatrix();
                glTranslatef(3.7f, 0.f, -2.1f);
                drawSolidCube(0.3f, 0.5f, 1.5f, 4, 6);
                glTranslatef(0.f, .65f, 0.f);
                glRotatef(90, 0.f, 1.f, 0.f);
                drawWedge(1.5f, 0.15f, 0.3f, 4, 0);
                glTranslatef(-0.6f, -0.14f, 0.15f);
                glScalef(0.2f, .2f, .2f);
                glRotatef(-26, 1.0f, 0.0f, 0.0f);
                drawScreen(1.0f, 1.0f, 1.0f, screenA);
            glTranslatef(5.f, 0.f, 0.f);
            drawScreen(2.0f, 1.0f, 1.0f, screenB);
            glPopMatrix();
            
            //FRONT WALL
            glPushMatrix();
                glTranslatef(0.f, 0.f, 2.4f);
                drawSolidCube(2.5f, 1.f, 0.101f, 4, 2);
                glTranslatef(0.f, 2.f, 0.f);
                drawSolidCube(2.5f, 2.f, 0.05f, 4, 3);
                glTranslatef(0.f, 2.f, -1.f);
                drawSolidCube(2.4f, 0.09f, 1.f, 4, 2);
            glPopMatrix();
            
            glPushMatrix();  // Left control Panel
                glTranslatef(-1.5f, 0.f, 0.f);
                glRotatef(10, 0.f, 1.f, 0.f);
                drawSolidCube(0.3f, 0.5f, 1.0f, 4, 6);
                glPushMatrix();
                    glScalef(0.4f, 0.4f, 0.4f);
                    glTranslatef(0.f, .8f, 1.f);
                    drawScreen(0.6f, 0.5f, .6f, button);
                    glTranslatef(0.f, 0.f, -2.f);
                    drawScreen(0.6f, 0.5f, .6f, button);
                glPopMatrix();
                glTranslatef(0.f, -0.501f, -0.95f);
                drawCylinder(.3f, 1.f, 15, 3, 0.f, 0.f, 0.f, 0);
            glPopMatrix();
                
                glPushMatrix();
                    glTranslatef(0.f, 0.5f, 1.f);
                    drawSolidCube(1.1f, 0.03f, 0.5f, 4, 0); //Desk Top
                        glPushMatrix();
                            glScalef(0.4f, 0.4f, 0.4f);
                            glTranslatef(-1.f, -0.4f, -0.6f);
                            drawScreen(1.0f, 0.5f, .5f, button);
                            glPushMatrix();
                                glTranslatef(2.0f, 0.5f, -0.3f);
                                drawCylinder(0.05f, 0.8f, 10, 5, 0.0f, 0.0f, 0.0f, 0);
                                glPushMatrix();
                                    glTranslatef(0.0f, 0.8f, 0.0f);
                                    glScalef(0.2f, 0.2f, 0.2f);
                                    drawSphere(1.f, 4, 5);
                                glPopMatrix();
                                glTranslatef(0.0f, 0.0f, 0.0f);
                                drawSolidCube(0.3f, 0.03f, 0.3f, 3, 2);
                            glPopMatrix();
                        glPopMatrix();
                        glTranslatef(0.0f, -0.5f, 0.1f);
                        drawSolidCube(1.1f, 0.5f, 0.4f, 4, 6); // Desk Underside Backing
                        glPushMatrix();
                            glTranslatef(0.f, .67f, 0.1f);
                            drawWedge(1.1f, 0.15f, 0.3f, 4, 0); //Centre Desk Wedge
                            glTranslatef(-0.8f, -0.14f, 0.15f);
                            glScalef(0.2f, .2f, .2f);
                            glRotatef(-26, 1.0f, 0.0f, 0.0f);
                            drawScreen(1.0f, 1.0f, 1.0f, screenA);  //Left Screen
                            glTranslatef(3.f, -0.f, 0.f);
                            drawScreen(1.0f, 1.0f, 1.0f, screenA); //Centre Screen
                            glTranslatef(4.f, -0.f, 0.f);
                            drawScreen(2.0f, 1.0f, 1.0f, screenB); //Centre Screen
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
       
        glPushMatrix();     //Steps
            glTranslatef(0.0f, -0.3f, -3.f);
            drawSolidCube(4.f, 0.15f, 0.15f, 5, 0);
            for (int i = 0; i <= 2; i++)
            {
                glTranslatef(0.0f, 0.3f, -.3f);
                drawSolidCube(4.f, 0.15f, 0.15f, 5, 0);
            }
        glPopMatrix();
        
        //Back Railings & Floor
        glPushMatrix();
            glTranslatef(0.0f, 0.7f, -4.55f);
            glPushMatrix();
                drawCylinder(0.07f, 1.f, 10, 10, 0.0f, 0.0f, 0.0f, 10);  //Middle Pole
                    glPushMatrix();
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                glPushMatrix();
                    glTranslatef(1.5f, 0.0f, 0.0f);
                    drawCylinder(0.07f, 1.f, 10, 10, 0.0f, 0.0f, 0.0f, 10);  //Right 1 Pole
                    glPushMatrix();
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                    glTranslatef(1.5f, 0.0f, 0.0f);
                    drawCylinder(0.07f, 1.f, 10, 10, 0.0f, 0.0f, 0.0f, 10);
                    glPushMatrix();
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(1.5f, 0.0f, 0.0f);
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix();
                    glTranslatef(-1.5f, 0.0f, 0.0f);
                    drawCylinder(0.07f, 1.f, 10, 10, 0.0f, 0.0f, 0.0f, 10);
                    glPushMatrix();
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                    glTranslatef(-1.5f, 0.0f, 0.0f);
                    drawCylinder(0.07f, 1.f, 10, 10, 0.0f, 0.0f, 0.0f,10);
                    glPushMatrix();
                        glTranslatef(-0.75f, 1.f, 0.f);
                        drawSolidCube(.75f, 0.02f, 0.07f, 4, 1);
                    glPopMatrix();
                glPopMatrix();
                glPushMatrix(); //Back Floor
                    glTranslatef(0.0f, 0.0f, -1.5f);
                    drawFloor(4.0f, 0.05f, 2.0f, 6,  floor, 10);
                glPopMatrix();
            glPopMatrix();
            
            glPushMatrix();  //Left Back Wall
                    glTranslatef(-4.f, 1.f, -1.f);
                    drawSolidCube(0.1f, 2.0f, 2.5f, 5, 0);
                    glRotatef(-90, 0.f, 1.f, 0.f);
                    glTranslatef(0.f, -0.9f, -.15f);
        
        glPushMatrix();  //Left wall pipes etc
        drawWedge(2.5f, 0.05f, 0.05f, 4, 0);
            glPushMatrix();
                glTranslatef(0.41f, 2.5f, -.1f);
                glRotatef(90.f, 0.f, 0.f, 1.f);
                drawCylinder(0.05f, 3.f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
                glTranslatef(0.04f, 0.04f, 0.f);
                glRotatef(90.f, 0.f, 0.f, 1.f);
                drawCylinder(0.05f, 0.65f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.25f, 2.35f, -.1f);
                glRotatef(90.f, 0.f, 0.f, 1.f);
                drawCylinder(0.05f, 2.8f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
                glTranslatef(0.04f, 0.04f, 0.f);
                glRotatef(90.f, 0.f, 0.f, 1.f);
                drawCylinder(0.05f, 0.5f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(1.15f, 1.85f, -.1f);
                glRotatef(90.f, 0.f, 0.f, 1.f);
                drawCylinder(0.05f, 1.f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
                glTranslatef(0.f, 0.1f, 0.f);
                glRotatef(90.f, 1.f, 0.f, 0.f);
                drawCylinder(0.05f, .2f, 10, 15, 0.0f, 0.0f, 0.0f, 10);
            glPopMatrix();
        
        glPopMatrix();
            glPopMatrix();
        
            glPushMatrix();  //Right Back Wall
                    glTranslatef(4.f, 1.f, -1.f);
                    drawSolidCube(0.1f, 2.0f, 2.5f, 5, 0);
                    glRotatef(90, 0.f, 1.f, 0.f);
                    glTranslatef(0.f, -0.9f, -.15f);
                    drawWedge(2.5f, 0.05f, 0.05f, 4, 0);
            glPopMatrix();
        
            glPushMatrix();  //Ceiling Back
                    glTranslatef(0.f, 3.f, -1.f);
                    drawSolidCube(4.f, 0.1f, 2.5f, 5, 0);
            glPopMatrix();
        
            glPushMatrix();  //Back Wall + Door Screens
                    glTranslatef(3.f, 1.f, -3.5f);
                    drawSolidCube(1.f, 2.f, .1f, 4, 0);
                    glPushMatrix();
                        glRotatef(90.f, 1.f, 0.f, 0.f);
                        glRotatef(180.f, 0.f, 1.f, 0.f);
                        glTranslatef(0.5f, -0.18f, 0.5f);
                        drawScreen(0.3f, 0.3f, 0.3f, door);
                    glPopMatrix();
                    glPushMatrix();
                        glRotatef(-90.f, 1.f, 0.f, 0.f);
                        glTranslatef(-0.5f, -0.18f, 0.5f);
                        drawScreen(0.3f, 0.3f, 0.3f, door);
                    glPopMatrix();
                    glTranslatef(-6.f, 0.f, 0.f);
                    drawSolidCube(1.f, 2.f, .1f, 4, 0);
                    glPushMatrix();
                        glRotatef(90.f, 1.f, 0.f, 0.f);
                        glRotatef(180.f, 0.f, 1.f, 0.f);
                        glTranslatef(-0.5f, -0.18f, 0.5f);
                        drawScreen(0.3f, 0.3f, 0.3f, door);
                    glPopMatrix();
                    glPushMatrix();
                        glRotatef(-90.f, 1.f, 0.f, 0.f);
                        glTranslatef(0.5f, -0.18f, 0.5f);
                        drawScreen(0.3f, 0.3f, 0.3f, door);
                    glPopMatrix();
            glPopMatrix();
        
            glPushMatrix();  //Back Room floor
                    glTranslatef(0.f, 0.f, -6.5f);
                    drawFloor(5.0f, 0.05f, 3.0f, 6,  floor, 10);
        
                glPushMatrix();  //Back Room Left Wall
                    glTranslatef(5.f, 0.f, 0.f);
                    drawSolidCube(0.1f, 1.f, 3.f, 4, 0);
                    glTranslatef(0.f, 2.f, 0.f);
                    drawSolidCube(0.1f, 1.f, 3.f, 4, 3);
                    glTranslatef(-1.2f, 1.f, 0.f);
                    drawSolidCube(1.2f, 0.1f, 3.f, 4, 0);
                    glTranslatef(.7f, -1.5f, 3.f);
                    drawSolidCube(.5f, 1.5f, 0.1f, 4, 0);
                glPopMatrix();
        
                glPushMatrix();  // Back Room Right Wall
                    glTranslatef(-5.f, 0.f, 0.f);
                    drawSolidCube(0.1f, 1.f, 3.f, 4, 0);
                    glTranslatef(0.f, 2.f, 0.f);
                    drawSolidCube(0.1f, 1.f, 3.f, 4, 3);
                    glTranslatef(1.2f, 1.f, 0.f);
                    drawSolidCube(1.2f, 0.1f, 3.f, 4, 0);
                    glTranslatef(-.7f, -1.5f, 3.f);
                    drawSolidCube(.5f, 1.5f, 0.1f, 4, 0);
                glPopMatrix();
            
                glPushMatrix();  //Back Room Back Wall + Doors
                    glTranslatef(0.f, 1.f, -3.f);
                    drawSolidCube(5.f, 2.f, 0.1, 5, 0);
                    glPushMatrix();
                        glTranslatef(-0.82f, 0.0f, 0.f);
                        drawSolidCube(0.8f, 1.8f, 0.15f, 3, 2);
                        glTranslatef(-1.2f, 0.5f, 0.f);
                        glRotatef(90.f, 1.f, 0.f, 0.f);
                        glRotatef(180.f, 0.f, 1.f, 0.f);
                        drawScreen(0.2f, 0.2f, 0.2f, door_closed);
                    glPopMatrix();
                    glPushMatrix();
                        glTranslatef(0.82f, 0.0f, 0.f);
                        drawSolidCube(0.8f, 1.8f, 0.15f, 3, 2);
                        glTranslatef(1.2f, 0.5f, 0.f);
                        glRotatef(90.f, 1.f, 0.f, 0.f);
                        glRotatef(180.f, 0.f, 1.f, 0.f);
                        drawScreen(0.2f, 0.2f, 0.2f, door_closed);
                    glPopMatrix();
                    drawSolidCube(0.04f, 1.8f, 0.15f, 3, 9);
                glPopMatrix();
        
                glPushMatrix();  //Back Room Ceiling Window
                    glTranslatef(0.0f, 3.f, 0.f);
                    drawSolidCube(3.f, 0.05f, 3.f, 4, 3);
                glPopMatrix();
           
                glPushMatrix(); //Robot
                    drawRobot(frame, pos);
                glPopMatrix();
        
            glPopMatrix();
            glPushMatrix();
                drawDoors(door_state, frame);
            glPopMatrix();
        glPopMatrix();
        glPopMatrix();
            
        /* Screen update timing */
        double now = glfwGetTime();
        glfwPollEvents();
        if ((now - lastFrameTime) >= fpsLimit)
        {
            glfwSwapBuffers(window);
            lastFrameTime = now;
            frame++;
        }
    }
    glfwTerminate();
    return 0;
}

bool initOpenGL()
{
    if (!glfwInit()) {
        std::cout << "GLFW initialisation failed." << std::endl;
        return false;
    }

    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (!window)
    {
        std::cout << "GLFW failed to create window." << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "GLEW initialisation failed: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "Esc key is pressed." << std::endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        wKey = true;
    }

    if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    {
        wKey = false;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        sKey = true;
    }

    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        sKey = false;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        aKey = true;
    }

    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        aKey = false;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        dKey = true;
    }

    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        dKey = false;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        torch = !torch;
    }
}

void downLight(int state) { //Provides a differents light state depending on state
    // 1 = Red Emergency Lighting
    // 2 = Regular White Light
    // 3 = No Light / Power Failure
    if (state == 1) {
    float ambient_1[] = { 0.5f, 0.f, 0.f, 1.0f };
    float diffuse_1[] = { 0.5f, 0.f, 0.f, 1.0f };
    float specular_1[] = { 0.5f, 0.f, 0.f, 1.0f };
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular_1);
    glEnable(GL_LIGHT1);
    float position_1[] = {0.f, 5.f, -3.f, 1.f};
    glLightfv(GL_LIGHT1, GL_POSITION, position_1);
    
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_1);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse_1);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular_1);
    glEnable(GL_LIGHT3);
    float position_3[] = { 0.f, 5.f, -8.f, 1.f };
    glLightfv(GL_LIGHT3, GL_POSITION, position_3);
    }
    
    else if (state == 2) {
        float ambient_1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
        float diffuse_1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        float specular_1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        
        float ambient_s[] = { 0.8f, 0.8f, 0.7f, 1.0f };
        float diffuse_s[] = { 0.8f, 0.8f, 0.7f, 1.0f };
        float specular_s[] = { 0.8f, 0.8f, 0.7f, 1.0f };
        
        float ambient_r[] = { 0.8f, 0.2f, 0.2f, 1.0f };
        float diffuse_r[] = { 0.8f, 0.2f, 0.2f, 1.0f };
        float specular_r[] = { 0.8f, 0.2f, 0.2f, 1.0f };

        //Front Room Point Light
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular_1);
        glEnable(GL_LIGHT1);
        float position_1[] = { -2.f, 4.f, 2.f, 1.f };
        glLightfv(GL_LIGHT1, GL_POSITION, position_1);
        
        //Stair Highlights
        GLfloat spot_dir1[] = { -1.f, -1.f, 0.f };
        GLfloat spot_cutoff1 = 40.0f;
        GLfloat spot_exponent1 = 10.f;
        glLightfv(GL_LIGHT2, GL_AMBIENT, ambient_s);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse_s);
        glLightfv(GL_LIGHT2, GL_SPECULAR, specular_s);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_dir1);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, spot_cutoff1);
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, spot_exponent1);
        glEnable(GL_LIGHT2);
        float position_2[] = { 4.f, 1.5f, -3.5f, 1.f };
        glLightfv(GL_LIGHT2, GL_POSITION, position_2);
        
        //Back Room Point
        glLightfv(GL_LIGHT3, GL_AMBIENT, ambient_1);
        glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse_1);
        glLightfv(GL_LIGHT3, GL_SPECULAR, specular_1);
        glEnable(GL_LIGHT3);
        float position_3[] = { 0.f, 2.f, -6.f, 1.f };
        glLightfv(GL_LIGHT3, GL_POSITION, position_3);
        
        //Robot Highlight
        GLfloat spot_dir2[] = { 0.f, -1.f, -1.f };
        GLfloat spot_cutoff2 = 30.0f;
        GLfloat spot_exponent2 = 20.f;
        glLightfv(GL_LIGHT4, GL_AMBIENT, ambient_s);
        glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse_s);
        glLightfv(GL_LIGHT4, GL_SPECULAR, specular_s);
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, spot_dir2);
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, spot_cutoff2);
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, spot_exponent2);
        glEnable(GL_LIGHT4);
        float position_4[] = { 1.f, 4.f, -8.f, 1.f };
        glLightfv(GL_LIGHT4, GL_POSITION, position_4);
        
        GLfloat spot_dir3[] = { 0.f, -1.f, 0.f };
        GLfloat spot_cutoff3 = 30.0f;
        GLfloat spot_exponent3 = 20.f;
        glLightfv(GL_LIGHT5, GL_AMBIENT, ambient_r);
        glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse_r);
        glLightfv(GL_LIGHT5, GL_SPECULAR, specular_r);
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, spot_dir3);
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, spot_cutoff3);
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, spot_exponent3);
        glEnable(GL_LIGHT5);
        float position_5[] = {0.f, 3.f, -11.f, 0.f };
        glLightfv(GL_LIGHT5, GL_POSITION, position_5);
        
        //Robot Highlight
        GLfloat spot_dir4[] = { 0.f, -1.f, 1.f };
        GLfloat spot_cutoff4 = 60.0f;
        GLfloat spot_exponent4 = 10.f;
        glLightfv(GL_LIGHT6, GL_AMBIENT, ambient_s);
        glLightfv(GL_LIGHT6, GL_DIFFUSE, diffuse_s);
        glLightfv(GL_LIGHT6, GL_SPECULAR, specular_s);
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_dir4);
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, spot_cutoff4);
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, spot_exponent4);
        glEnable(GL_LIGHT6);
        float position_6[] = { 1.f, 4.f, -15.f, 1.f };
        glLightfv(GL_LIGHT6, GL_POSITION, position_6);
        
    }
        else if (state == 3) {
            //LIGHT 0 = Torch
            //Light 7 = Sun
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
            glDisable(GL_LIGHT4);
            glDisable(GL_LIGHT5);
            glDisable(GL_LIGHT6);
    }
}

int light_state(int frame)
{ //Calculates light state from frame
    int loop_position = frame % 1200;
    if (loop_position < 300) {
        return 2;
    }
    else if (loop_position >= 300 && loop_position < 330) {
        return 3;
    }
    else if (loop_position >= 330 && loop_position < 360) {
        return 2;
    }
    else if (loop_position >= 360 && loop_position < 400) {
        return 3;
    }
    else if (loop_position >= 400 && loop_position < 420) {
        return 2;
    }
    else if (loop_position >= 420 && loop_position < 900) {
        return 3;
    }
    else if (loop_position >= 900 && loop_position < 1200) {
        return 1;
    }
    return 2;
}

glm::vec3 position_manager(glm::vec3 pos)
{   //Stops the camera from travelling out of bounds of the ship
    pos[1] = 2.5f;
    
    if (pos[2] > -4.7f) {   //Railing Block
        pos[2] = -4.7f;
    }
    if (pos[2] < -4.5f && pos[2] > -8.f) {   //Start Room Side Walls
        if (pos[0] > 3.4f){
            pos[0] = 3.4f;
        }
        if (pos[0] < -3.4f){
            pos[0] = -3.4f;
        }
    }
    if (pos[0] > -4.f && pos[0] < -1.5f && pos[2] > -8.f)  //Start Room Back Wall
    {
        if (pos[2] < -7.8f){
            pos[2] = -7.8f;
        }
    }
    if (pos[0] < 4.f && pos[0] > 1.5f && pos[2] > -8.f) //Start Room Back Wall
    {
        if (pos[2] < -7.8f){
            pos[2] = -7.8f;
        }
    }
    if (pos[0] < 5.f && pos[0] > 1.f && pos[2] < -8.1f) //Back Room Front Wall
    {
        if (pos[2] > -8.3f){
            pos[2] = -8.3f;
        }
    }
    if (pos[0] > -5.f && pos[0] < -1.f && pos[2] < -8.1f) //Back Room Front Wall
    {
        if (pos[2] > -8.3f){
            pos[2] = -8.3f;
        }
    }
    if (pos[2] < -8.2f && pos[2] > -16.f) {   //Back Room Side Walls
        if (pos[0] > 4.8f){
            pos[0] = 4.8f;
        }
        if (pos[0] < -4.8f){
            pos[0] = -4.8f;
        }
    }
    if (pos[2] < -13.7f) {   //Back Room Back Wall
        pos[2] = -13.7f;
    }
    return pos;
}

glm::vec4 generateStarfield(float minDistance, float maxDistance)
{   //Populates the starfield array.
    while (true)
    {
        glm::vec4 position;
        float x = (rand() % (100 + 25)) - 50;
        float y = (rand() % (100 + 25)) - 50;
        float z = (rand() % (100 + 25)) - 50;
        position[0] = x;
        position[1] = y;
        position[2] = z;
        position[3] = static_cast <float> ((rand()) / static_cast <float> (RAND_MAX)) * 0.5;
        float len = glm::length(position);
        //Randomly generated point is returned if its length from origin is within the allowed radiuses
        if (len > minDistance && len < maxDistance)
        {
            return position;
        }
    }
}
