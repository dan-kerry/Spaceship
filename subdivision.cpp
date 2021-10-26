#include "subdivision.hpp"

void planeDivision(GLfloat x, GLfloat y, GLfloat z, int depth, int scale)
{
    //Creates a textured plane with subdivisions to allow for more accurate lighting
    const float points[4][3] = { {-x, y, z}, {-x, y, -z}, {x, y, -z}, {x, y, z} };
    float* newPoints[3];
    
    newPoints[0] = const_cast<float*>(points[0]);
    newPoints[1] = const_cast<float*>(points[1]);
    newPoints[2] = const_cast<float*>(points[2]);
    subdivideFlat(newPoints, depth - 1, scale);
    
    newPoints[0] = const_cast<float*>(points[2]);
    newPoints[1] = const_cast<float*>(points[3]);
    newPoints[2] = const_cast<float*>(points[0]);
    subdivideFlat(newPoints, depth - 1, scale);
}

void shapeDivision(GLfloat point1[], GLfloat point2[], GLfloat point3[], int depth)
{
    //Creates a subdivided triangle for more complex shapes
    float* points[3];
    points[0] = const_cast<float*>(point1);
    points[1] = const_cast<float*>(point2);
    points[2] = const_cast<float*>(point3);

    float* newPoints[3];
    newPoints[0] = const_cast<float*>(points[0]);
    newPoints[1] = const_cast<float*>(points[1]);
    newPoints[2] = const_cast<float*>(points[2]);
    subdivideFlat(newPoints, depth - 1, 0);
}

void subdivideFlat(GLfloat* points[], int depth, int scale)
{
    //Subdivides flat surfaces (e.g. Screens and Floors)
    if (depth == 0) { 
        drawFlatTriangle(points, scale);
        return;
    }
 
    float midpoints[3][3];
    midPoint(midpoints[0], points[0], points[1]);
    midPoint(midpoints[1], points[1], points[2]);
    midPoint(midpoints[2], points[2], points[0]);
    
    float *newPoints[3];
    newPoints[0] = points[0];
    newPoints[1] = midpoints[0];
    newPoints[2] = midpoints[2];
    subdivideFlat(newPoints, depth-1, scale);
    newPoints[0] = points[1];
    newPoints[1] = midpoints[1];
    newPoints[2] = midpoints[0];
    subdivideFlat(newPoints, depth-1, scale);
    newPoints[0] = points[2];
    newPoints[1] = midpoints[2];
    newPoints[2] = midpoints[1];
    subdivideFlat(newPoints, depth-1, scale);
    newPoints[0] = midpoints[0];
    newPoints[1] = midpoints[1];
    newPoints[2] = midpoints[2];
    subdivideFlat(newPoints, depth-1, scale);
}

void subDivideComplex(GLfloat* points[], int depth, int flag)
{   //Subdivides surfaces that need subdivision and smoothing
    //Flag is passed on to draw function to determine draw process
    if (depth == 0) { 
        drawComplexTriangle(points, flag);
        return;
    }
    //get midpoints
    float midpoints[3][3];
    midPoint(midpoints[0],points[0], points[1]);
    midPoint(midpoints[1],points[1], points[2]);
    midPoint(midpoints[2],points[2], points[0]);
    normalise(midpoints[0]);
    normalise(midpoints[1]);
    normalise(midpoints[2]);

    //subdivide
    float *newPoints[3];
    newPoints[0] = points[0];
    newPoints[1] = midpoints[0];
    newPoints[2] = midpoints[2];
    subDivideComplex(newPoints,depth-1, flag);
    newPoints[0] = points[1];
    newPoints[1] = midpoints[1];
    newPoints[2] = midpoints[0];
    subDivideComplex(newPoints,depth-1, flag);
    newPoints[0] = points[2];
    newPoints[1] = midpoints[2];
    newPoints[2] = midpoints[1];
    subDivideComplex(newPoints,depth-1, flag);
    newPoints[0] = midpoints[0];
    newPoints[1] = midpoints[1];
    newPoints[2] = midpoints[2];
    subDivideComplex(newPoints,depth-1, flag);
}

void drawComplexTriangle(GLfloat** points, int flag) {
    if (flag == 0)  //Flag 0 = Apply texture to triangle and reverse winding order for skydome
    {
        float PI = 3.141592654;
        
        GLfloat a1[3];
        a1[0] = points[0][0];
        a1[1] = points[0][1];
        a1[2] = points[0][2];
        float u = atan2(a1[0], abs(a1[2])) / (2*PI) + 0.5;
        float v = a1[1] * 0.5 + 0.5;
    
        GLfloat a2[3];
        a2[0] = points[1][0];
        a2[1] = points[1][1];
        a2[2] = points[1][2];
        float u2 = atan2(a2[0], abs(a2[2])) / (2*PI) + 0.5;
        float v2 = a2[1] * 0.5 + 0.5;
     
        GLfloat a3[3];
        a3[0] = points[2][0];
        a3[1] = points[2][1];
        a3[2] = points[2][2];
        float u3 = atan2(a3[0], abs(a3[2])) / (2*PI) + 0.5;
        float v3 = a3[1] * 0.5 + 0.5;
        

        glBegin(GL_TRIANGLES);
        glPushMatrix();
            glTexCoord2f(u, v);
            glVertex3fv(points[0]);
            glTexCoord2f(u2, v2);
            glVertex3fv(points[1]);
            glTexCoord2f(u3, v3);
            glVertex3fv(points[2]);
        glPopMatrix();
        glEnd();
        }
    
        else if (flag == 1)
        {   //Flag 1 = Draw triangle for regular sphere.
            glBegin(GL_TRIANGLES);
            glPushMatrix();
                glVertex3fv(points[2]);
                glVertex3fv(points[1]);
                glVertex3fv(points[0]);
            glPopMatrix();
            glEnd();
        }
    }

void drawFlatTriangle(GLfloat** points, int scale) {
    if (scale > 0)  //If a texture is being used, then the approproate scale is applied
    {
        float a = ((points[0][0] + 3) /6) * scale;
        float b = ((points[0][2] + 3) /6) * scale;
        float c = ((points[1][0] + 3) /6) * scale;
        float d = ((points[1][2] + 3) /6) * scale;
        float e = ((points[2][0] + 3) /6) * scale;
        float f = ((points[2][2] + 3) /6) * scale;
        
        glBegin(GL_TRIANGLES);
        glPushMatrix();
            glTexCoord2f(a, b);
            glVertex3fv(points[0]);
            glTexCoord2f(c, d);
            glVertex3fv(points[1]);
            glTexCoord2f(e, f);
            glVertex3fv(points[2]);
        glPopMatrix();
        glEnd();
    }
    
    else {
        glBegin(GL_TRIANGLES);
        glVertex3fv(points[0]);
        glVertex3fv(points[1]);
        glVertex3fv(points[2]);
        glEnd();
    }
}

void normalise(float v[3])
{
    //Normalises an inputted array
    GLfloat d = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= (d/1.f);
    v[1] /= (d/1.f);
    v[2] /= (d/1.f);
}

void midPoint(GLfloat out[],GLfloat point1[], GLfloat point2[])
{   //Used by both subdivision processes to find midpoint of two points
    out[0] = (point1[0] + point2[0]) / 2.f;
    out[1] = (point1[1] + point2[1]) / 2.f;
    out[2] = (point1[2] + point2[2]) / 2.f;
}
