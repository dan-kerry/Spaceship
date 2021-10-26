#ifndef subdivision_hpp
#define subdivision_hpp
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <cstdlib>

#include <stdio.h>
void planeDivision(float x, float y, float z, int depth, int scale);
void shapeDivision(GLfloat point1[], GLfloat point2[], GLfloat point3[], int depth);
void subdivideFlat(GLfloat* points[], int depth, int scale);
void subDivideComplex(GLfloat* points[], int depth, int flag);
void drawComplexTriangle(GLfloat** points, int flag);
void drawFlatTriangle(GLfloat** points, int scale);
void midPoint(GLfloat out[],GLfloat point1[], GLfloat point2[]);
void normalise(float v[3]);
#endif
