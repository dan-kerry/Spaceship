#ifndef drawing_hpp
#define drawing_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

void drawSolidCube(float x, float y, float z, int subs, int mat);
void drawCylinder(float r, float HEIGHT, int slices,int segs, float x, float y, float z, int mat);
void drawSkybox(float r);
void drawFloor(float x, float y,float z,int divs, GLuint tex, int scale);
void drawWedge(float x, float y, float z, int subs, int mat);
void drawScreen(float x, float y, float z, GLuint tex);
void drawSkydome(float s, int depth, GLuint tex);
void drawRobot(int state, glm::vec3 pos);
void drawSphere(float s, int depth, int mat);
void drawDoors(int status, int frame);
void drawDisc(float size, int mat);
void drawStarfield(glm::vec4 starPos, glm::vec3 pos);

#endif /* drawing_hpp */
