#include "drawing.hpp"
#include "subdivision.hpp"

//All materials used are standardised in array, to prevent repetition and ensure consistency
float materials[11][5][4] = {
    //0: White Metal
    { {.1f, 0.1f, 0.1f, 1.f}, {.5f, 0.5f, 0.5f, 0.5f}, {0.8f, 0.8f,                        0.8f, 0.5f}, {120.0f}, {0.0f, 0.f, 0.f, 1.f} },
    //1:Yellow Plastic
    { {0.f, 0.f, 0.f, 1.f}, {.5f, 0.5f, 0.3f, 1.f}, {0.1f, 0.1f, 0.1f, 1.f}, {32.0f}, {0.0f, 0.f, 0.f, 1.f} },
    //2:Black Plastic
    { {0.2f, 0.2f, 0.2f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.2f, 0.2f, 0.2f, 1.f}, {62.0f}, {0.0f, 0.f, 0.f, 1.f} },
    //3:Glass
    { {0.1f, 0.1f, 0.1f, .2f}, {.6f, 0.6f, 0.6f, .4f}, {1.f, 1.f, 1.f, 1.f}, {122.0f}, {0.0f, 0.f, 0.f, 1.f} },
    //4:Red Glow
    { {0.1f, 0.1f, 0.1f, .2f}, {.6f, 0.0f, 0.0f, .4f}, {1.f, 0.f, 0.f, 1.f}, {122.0f}, {1.0f, 0.f, 0.f, 1.f} },
    //5:Red Leather
    { { 0.17f, 0.011f, 0.011f, 0.55f }, {0.61f, 0.04f, 0.04f, 0.55f }, {0.72f, 0.62f, 0.62f, 0.55f }, {76.f}, {0.0f, 0.f, 0.f, 1.f} },
    //6:White Plastic
    { {0.1f,0.1f,0.1f,1.0f}, {0.75f,0.75f,0.75f,1.0f}, {0.70f,0.70f,0.70f,1.0f}, {122.f}, {0.f, 0.f, 0.f, 1.f}},
    //7:Sun Orange
        { {0.f,0.f,0.f,1.0f}, {0.f,0.f,0.f,1.0f}, {0.f,0.f,0.f,1.0f}, {122.f}, {1.f, .8f, .3f, 1.f}},
    //8:Star White
        { {0.1f,0.1f,0.1f,1.0f}, {0.75f,0.75f,0.75f,1.0f}, {0.70f,0.70f,0.70f,1.0f}, {122.f}, {1.f, 1.f, 1.f, 1.f}},
    //9:Shiny Black
        { {0.f,0.f,0.f,1.0f}, {0.f,0.f,0.f,1.0f}, {0.70f,0.70f,0.70f,1.0f}, {122.f}, {0.f, 0.f, 0.f, 1.f}},
    //10:Shiny Silver
        { {0.f,0.f,0.f,1.0f}, {0.3f,0.3f,0.3f,1.0f}, {0.70f,0.70f,0.70f,1.0f}, {122.f}, {0.f, 0.f, 0.f, 1.f}}
    
};

int prev_door = 0;
int init_frame;
bool move;

void drawDoors(int status, int frame)
{   //Doors are drawn dependent on pre-calculated door status (see main.cpp)
    //Status 0 = Closed
    //Status 1 = Open
    bool active;
    float door;
    
    if (status != prev_door && move == false)
    {
        move = true;
        init_frame = frame;
    }
    if (move)
    {
        int anim_frame = frame - init_frame;
        active = true;
        if (status == 0)
        {
            door = 1.5f - (anim_frame * 0.025);
        }
        else if (status == 1)
        {
            door = 0.0f + (anim_frame * 0.025);
        }
        
        if (anim_frame > 59)
        {
            move = false;
            active = false;
            prev_door = status;
            init_frame = 0;
        }
    }
   
    else
    {
        if (status == 0)
        {
            door = 0.f;
        }
        else if (status == 1)
        {
            door = 1.5f;
        }
    }
    glPushMatrix();
        glTranslatef(0.f, 1.f, -3.5f);
        glPushMatrix();
            glTranslatef(1.f, 0.f, 0.f);
            glTranslatef(door, 0.f, 0.f);
            drawSolidCube(1.f, 2.f, 0.05f, 3, 3);
            glTranslatef(-1.f, 0.f, 0.f);
            drawSolidCube(0.02f, 2.f, 0.05f, 3, 0);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.f, 0.f, 0.f);
            glTranslatef(-door, 0.f, 0.f);
            drawSolidCube(1.f, 2.f, 0.05f, 3, 3);
            glTranslatef(1.f, 0.f, 0.f);
            drawSolidCube(0.02f, 2.f, 0.05f, 3, 0);
        glPopMatrix();
    glPopMatrix();
}

glm::vec3 saved_pos = glm::vec3(0.0f, 0.0f, 0.0f);
void drawRobot(int frame, glm::vec3 pos)
{
    glm::vec3 robot_pos = glm::vec3(0.0f, 0.0f, -11.05f);
    float playerAngleRads = atan2(pos[0]-robot_pos[0], pos[2]-robot_pos[2]);
    float playerAngle = playerAngleRads * (180.0/3.141592653589793238463);
    float playerSavedAngleRads = atan2(saved_pos[0]-robot_pos[0], saved_pos[2]-robot_pos[2]);
    float playerSavedAngle = playerSavedAngleRads * (180.0/3.141592653589793238463);
    float bodyHeight = 1.2f;
    float upper_legA = 30.f;
    float lower_leg_A = 30.f;;
    float lower_leg_p = .7f;;
    float lower_leg_AX = 0.f;
    float bodyRotate = 0.f;
    float upper_legAY = 0.f;
    float headRotate = 0.f;
    float legA = 0.0f;
    float legB = 0.0f;
    float headXRot = 0.0f;
    float bodyX = 0.0f;
    float bodyZ = 0.0f;
    int loop_pos = frame % 1100;
    
    if (loop_pos < 30)  //Initial Position
    {
        bodyHeight = 1.2f;
        lower_leg_p = .7f;
        lower_leg_A = 30.f;
    }
    else if (loop_pos >= 30 && loop_pos < 90) //Lower Body
    {
        bodyHeight = 1.2f - (((loop_pos + 30)-60) * 0.0133f);
        upper_legA = 30.f - (((loop_pos + 30)-60) * .7f);
        lower_leg_p = 0.7f - (((loop_pos + 30)-60) * 0.0033f);
        lower_leg_A = 30.f + (((loop_pos + 30)-60) * 1.f);
    }
    else if (loop_pos >= 90 && loop_pos < 110) //Pause
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
    }
    else if (loop_pos >= 110 && loop_pos < 140) //Turn Left
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        lower_leg_AX = 0.f + ((loop_pos - 110) * 1.f);
        bodyRotate = 0.f - ((loop_pos - 110) * 0.66f);
        upper_legAY = 0.f - ((loop_pos - 110) * 0.2f);;
    }
    else if (loop_pos >= 140 && loop_pos < 160) //Pause
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        lower_leg_AX = 30.f;
        bodyRotate = -20.f;
        upper_legAY = -6.f;
    }
    
    else if (loop_pos >= 160 && loop_pos < 190) //Turn Right
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        lower_leg_AX = 30.f - ((loop_pos - 160) * 2.f);
        bodyRotate = -20.f + ((loop_pos - 160) * 1.32f);
        upper_legAY = -6.f + ((loop_pos - 160) * 0.4f);;
    }
    else if (loop_pos >= 190 && loop_pos < 210) //Pause
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        lower_leg_AX = -30.f;
        bodyRotate = 20.f;
        upper_legAY = 6.f;
    }
    else if (loop_pos >= 210 && loop_pos < 240) //Return to centre
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        lower_leg_AX = -30.f + ((loop_pos - 210) * 1.f);
        bodyRotate = 20.f - ((loop_pos - 210) * 0.66f);
        upper_legAY = 6.f - ((loop_pos - 210) * 0.2f);;
    }
    else if (loop_pos >= 240 && loop_pos < 260) //Pause
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
    }
    
    else if (loop_pos >= 260 && loop_pos < 290) //Raise Body
    {
        bodyHeight = 0.4f + ((loop_pos - 260) * 0.0266f);
        upper_legA = -12.f + ((loop_pos  - 260) * 1.4f);
        lower_leg_p = .5f + ((loop_pos  - 260) * 0.0066f);
        lower_leg_A = 90.f - ((loop_pos  - 260) * 2.f);
    }
    else if (loop_pos >= 290 && loop_pos < 300) //Pause
    {
        bodyHeight = 1.2f;
        lower_leg_p = .7f;
        lower_leg_A = 30.f;
        upper_legA = 30.f;
    }
    else if (loop_pos >= 300 && loop_pos < 320) //Rotate head to camera
    {
        bodyHeight = 1.2f;
        lower_leg_p = .7f;
        lower_leg_A = 30.f;
        upper_legA = 30.f;
        headRotate = 0.0f + ((loop_pos  - 300) * (playerAngle/20));
        
    }
    else if (loop_pos >= 320 && loop_pos < 500) //Follow Camera
    {
        bodyHeight = 1.2f;
        lower_leg_p = .7f;
        lower_leg_A = 30.f;
        upper_legA = 30.f;
        headRotate = playerAngle;
        if (loop_pos == 499)    //Save a fixed location for camera
        {
            saved_pos = pos;
        }
    }
    
    else if (loop_pos >= 500 && loop_pos < 550) //Return to centre
    {
        bodyHeight = 1.2f;
        lower_leg_p = .7f;
        lower_leg_A = 30.f;
        upper_legA = 30.f;
        headRotate = playerAngle - ((loop_pos  - 500) * (playerAngle/50));
    }
    
    else if (loop_pos >= 550 && loop_pos < 610) //Rotate and lower to last known camera pos
    {
        bodyHeight = 1.2f - ((loop_pos - 550) * 0.0133f);
        upper_legA = 30.f - ((loop_pos - 550) * .7f);
        lower_leg_p = 0.7f - ((loop_pos - 550) * 0.0033f);
        lower_leg_A = 30.f + ((loop_pos - 550) * 1.f);
        bodyRotate = 0.0f + ((loop_pos  - 550) * (playerSavedAngle/60));
        bodyZ = 0.0f;
    }
    else if (loop_pos >= 610 && loop_pos < 625)//Pause
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        bodyRotate = playerSavedAngle;
        bodyZ = 0.0f;
    }
    else if (loop_pos >= 625 && loop_pos < 725) //Walk to camera
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        bodyRotate = playerSavedAngle;
        bodyZ = ((loop_pos  - 625) * (1.5f/100));
        if  (loop_pos >= 625 && loop_pos < 650)
        {
            legA = 0.0f + ((loop_pos  - 625) * (20.f/25));
            legB = 0.0f - ((loop_pos  - 625) * (20.f/25));
        }
        else if (loop_pos >= 650 && loop_pos < 675)
        {
            legA = 20.0f - ((loop_pos  - 650) * (20.f/25));
            legB = -20.0f + ((loop_pos  - 650) * (20.f/25));
        }
        else if (loop_pos >= 675 && loop_pos < 700)
        {
            legA = 0.0f + ((loop_pos  - 675) * (20.f/25));
            legB = 0.0f - ((loop_pos  - 675) * (20.f/25));
        }
        else if (loop_pos >= 700 && loop_pos < 725)
        {
            legA = 20.0f - ((loop_pos  - 700) * (20.f/25));
            legB = -20.0f + ((loop_pos  - 700) * (20.f/25));
        }
    }
    else if (loop_pos >= 725 && loop_pos < 900) //Look up and down camera
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        bodyRotate = playerSavedAngle;
        bodyZ = 1.5f;
        if (loop_pos >= 735 && loop_pos < 765)
        {
            headXRot = 0.0f + ((loop_pos  - 735) * (20.f/30));
        }
        else if (loop_pos >= 765 && loop_pos < 790)
        {
            headXRot = 20.f;
        }
        else if (loop_pos >= 790 && loop_pos < 840)
        {
            headXRot = 20.f - ((loop_pos  - 790) * (40.f/50));
        }
        else if (loop_pos >= 840 && loop_pos < 880)
        {
            headXRot = -20.f;
        }
        else if (loop_pos >= 880 && loop_pos < 900)
        {
            headXRot = -20.f + ((loop_pos  - 880) * (20.f/20));;
        }
    }
    else if (loop_pos >= 900 && loop_pos < 1000)    //Return to centre
    {
        bodyHeight = 0.4f;
        upper_legA = -12.f;
        lower_leg_A = 90.f;
        lower_leg_p = .5f;
        bodyRotate = playerSavedAngle;
        bodyZ = 1.5f - ((loop_pos  - 900) * (1.5f/100));
        if  (loop_pos >= 900 && loop_pos < 925)
        {
            legA = 0.0f + ((loop_pos  - 900) * (20.f/25));
            legB = 0.0f - ((loop_pos  - 900) * (20.f/25));
        }
        else if (loop_pos >= 925 && loop_pos < 950)
        {
            legA = 20.0f - ((loop_pos  - 925) * (20.f/25));
            legB = -20.0f + ((loop_pos  - 925) * (20.f/25));
        }
        else if (loop_pos >= 950 && loop_pos < 975)
        {
            legA = 0.0f + ((loop_pos  - 950) * (20.f/25));
            legB = 0.0f - ((loop_pos  - 950) * (20.f/25));
        }
        else if (loop_pos >= 975 && loop_pos < 1000)
        {
            legA = 20.0f - ((loop_pos  - 975) * (20.f/25));
            legB = -20.0f + ((loop_pos  - 975) * (20.f/25));
        }
    }
    else if (loop_pos >= 1000 && loop_pos < 1060) //Return rotation and height
    {
        bodyHeight = 0.4f + ((loop_pos - 1000) * 0.0133f);;
        upper_legA = -12.f + ((loop_pos - 1000) * .7f);
        lower_leg_A = 90.f - ((loop_pos - 1000) * 1.f);
        lower_leg_p = .5f + ((loop_pos - 1000) * 0.0033f);
        bodyRotate = playerSavedAngle - ((loop_pos  - 1000) * (playerSavedAngle/60));
    }
    
    glPushMatrix();
        glRotatef(bodyRotate, 0.f, 1.f, 0.f);
        glTranslatef(bodyX, 0.0f, bodyZ);
        glTranslatef(0.f, bodyHeight, 0.f);
        drawSolidCube(.2f, .2f, .2f, 4, 10); //Torso
        glPushMatrix();
            glTranslatef(0.f, .7f, 0.f);
            glRotatef(headRotate, 0.f, 1.f, 0.f);
            glRotatef(headXRot, 1.0f, 0.0f, 0.0f);
            drawSolidCube(0.3f, 0.5f, 0.3f, 4, 10);  //Head
            glPushMatrix();
                glTranslatef(0.f, 0.3f, 0.3f);
                glScalef(0.2f, 0.2f, 0.2f);
                drawSphere(1.f, 4, 4); //Eye
            glPopMatrix();
            glPushMatrix();
                glScalef(.5f, 1.f, .6f);
                drawSphere(1.f, 4, 3);  //Shield
            glPopMatrix();
        glPopMatrix();
        
        glPushMatrix();  //Leg 1
            glRotatef(legA, 0.0f, 1.0f, 0.0f);
            glRotatef(upper_legA, 0.0f, 0.f, 1.f);
            glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
            glTranslatef(-.5f, 0.f, 0.f);
            drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9);
            glPushMatrix();
                glTranslatef(-lower_leg_p, -0.2f, 0.f);
                glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
                glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
                drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9);
            glPopMatrix();
        glPopMatrix();
    
        glPushMatrix(); //Leg 2
            glRotatef(60, 0.0f, 1.f, 0.f);
            glRotatef(legB, 0.0f, 1.0f, 0.0f);
            glRotatef(upper_legA, 0.0f, 0.f, 1.f);
            glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
            glTranslatef(-.5f, 0.f, 0.f);
            drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9); //Upper Leg
            glPushMatrix();
                glTranslatef(-lower_leg_p, -0.2f, 0.f);
                glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
                glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
                drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9); //Lower Leg
            glPopMatrix();
        glPopMatrix();
    
        glPushMatrix();//Leg 3
            glRotatef(120, 0.0f, 1.f, 0.f);
    glRotatef(legB, 0.0f, 1.0f, 0.0f);
            glRotatef(upper_legA, 0.0f, 0.f, 1.f);
    glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
            glTranslatef(-.5f, 0.f, 0.f);
            drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9);
            glPushMatrix();
                glTranslatef(-lower_leg_p, -0.2f, 0.f);
                glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
                glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
                drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9);
            glPopMatrix();
        glPopMatrix();
    
    glPushMatrix();//Leg 4
        glRotatef(180, 0.0f, 1.f, 0.f);
    glRotatef(legA, 0.0f, 1.0f, 0.0f);
        glRotatef(upper_legA, 0.0f, 0.f, 1.f);
    glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
        glTranslatef(-.5f, 0.f, 0.f);
        drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9);
        glPushMatrix();
            glTranslatef(-lower_leg_p, -0.2f, 0.f);
            glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
            glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
            drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9);
        glPopMatrix();
    glPopMatrix();
        
    glPushMatrix();//Leg 5
        glRotatef(240, 0.0f, 1.f, 0.f);
    glRotatef(legB, 0.0f, 1.0f, 0.0f);
        glRotatef(upper_legA, 0.0f, 0.f, 1.f);
    glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
        glTranslatef(-.5f, 0.f, 0.f);
        drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9);
        glPushMatrix();
            glTranslatef(-lower_leg_p, -0.2f, 0.f);
            glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
            glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
            drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9);
        glPopMatrix();
    glPopMatrix();
    
    glPushMatrix();//Leg 6
        glRotatef(300, 0.0f, 1.f, 0.f);
    glRotatef(legB, 0.0f, 1.0f, 0.0f);
        glRotatef(upper_legA, 0.0f, 0.f, 1.f);
    glRotatef(upper_legAY, 0.0f, 1.f, 0.f);
        glTranslatef(-.5f, 0.f, 0.f);
        drawSolidCube(0.5f, 0.1f, 0.1f, 3, 9);
        glPushMatrix();
            glTranslatef(-lower_leg_p, -0.2f, 0.f);
            glRotatef(lower_leg_A, 0.0f, 0.f, 1.f);
            glRotatef(lower_leg_AX, 0.0f, 1.f, 0.f);
            drawSolidCube(0.3f, 0.05f, 0.05f, 3, 9);
        glPopMatrix();
    glPopMatrix();   
    glPopMatrix();
}

void drawSolidCube(float x, float y, float z, int subs, int mat)
{
    if (mat == 3)   //Adds neccesary settings to draw glass correctly
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask( GL_FALSE );
    }
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[mat][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[mat][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[mat][2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[mat][3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, materials[mat][4] );
    
    float points[8][3] = {{x, y, -z}, {-x, y, -z}, {-x, -y, -z}, {x, -y, -z},
                          {x, y, z}, {-x, y, z}, {-x, -y, z}, {x, -y, z}};
    // Back face
    glNormal3f(0.f, 0.f, 1.f);
    shapeDivision(points[5], points[4], points[7], subs);
    shapeDivision(points[7], points[6], points[5], subs);

    // Near face
    glNormal3f(0.f, 0.f, -1.f);
    shapeDivision(points[0], points[1], points[2], subs);
    shapeDivision(points[3], points[0], points[2], subs);

    // Right face
    glNormal3f(1.f, 0.f, 0.f);
    shapeDivision(points[0], points[3], points[7], subs);
    shapeDivision(points[7], points[4], points[0], subs);

    // Left face
    glNormal3f(-1.f, 0.f, 0.f);
    shapeDivision(points[6], points[2], points[1], subs);
    shapeDivision(points[1], points[5], points[6], subs);

    // Top face
    glNormal3f(0.f, 1.f, 0.f);
    shapeDivision(points[1], points[0], points[4], subs);
    shapeDivision(points[4], points[5], points[1], subs);

    // Bottom face
    glNormal3f(0.f, -1.f, 0.f);
    shapeDivision(points[2], points[6], points[3], subs);
    shapeDivision(points[3], points[6], points[7], subs);

    glPopAttrib();
    if (mat == 3) {
    glDepthMask( GL_TRUE );
    glDisable( GL_BLEND );}
}

void drawWedge(float x, float y, float z, int subs, int mat)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[mat][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[mat][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[mat][2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[mat][3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, materials[mat][4]);
    
    float points[6][3] = {{-x, y, z}, {-x, -y, z}, {-x, -y, -z},
                          {x, y, z}, {x, -y, z}, {x, -y, -z}};
    // Left face
    glNormal3f(-1.f, 0.f, 0.0f);
    shapeDivision(points[0], points[1], points[2], subs);

    // Right face
    glNormal3f(1.f, 0.f, 0.f);
    shapeDivision(points[3], points[5], points[4], subs);

    // Back face
    glNormal3f(0.f, 0.f, 1.f);
    shapeDivision(points[0], points[3], points[1], subs);
    shapeDivision(points[1], points[3], points[4], subs);

    // Bottom face
    glNormal3f(0.f, -1.f, 0.f);
    shapeDivision(points[1], points[4], points[2] , subs);
    shapeDivision(points[2], points[4], points[5], subs);

    // Angled face
    glNormal3f(0.f, 1.f, -1.f);
    shapeDivision(points[0], points[2], points[3], subs);
    shapeDivision(points[2], points[5], points[3], subs);

    glPopAttrib();
}
void drawCylinder(float r, float HEIGHT, int slices,int segs, float x, float y, float z, int mat)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[mat][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[mat][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[mat][2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[mat][3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, materials[mat][4]);

    GLfloat pi = 3.141592;
    GLfloat twicePi = 2.0f * pi;
    
    //Bottom
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f,-1.0f, 0.0f);
    glVertex3f(x, y , z);
    for(int i = slices; i >= 0; i--) {
        glVertex3f(x + (r * cos(i *  twicePi / slices)), y, z + ((r * sin(i * twicePi / slices))));
    }
    glEnd();
    
    //Top
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f,1.0f, 0.0f);
    glVertex3f(x,y + HEIGHT,z);
    for(int i = 0; i <= slices; i++) {
        glVertex3f(x + (r * cos(i *  twicePi / slices)), y + HEIGHT, z + ((r * sin(i * twicePi / slices))));
    }
    glEnd();

    // Sides - To add more detail the sides are split into sections and repeated
    for(int j=1; j<= segs; j++)
    {
        glBegin(GL_TRIANGLE_STRIP);
        for(int i=0; i<=slices; i++)
        {
                glNormal3f(x + cos((float)i/slices * 2 *pi)*r, 0.0f, z+sin((float)i/slices * 2 * pi)*r);
                glVertex3f(x + cos((float)i/slices * 2 *pi)*r,y + (j*(HEIGHT/segs)), z+sin((float)i/slices * 2 *pi)*r);
                glVertex3f(x + cos((float)i/slices * 2 *pi)*r, y + ((j-1)*(HEIGHT/segs)), z+sin((float)i/slices * 2 * pi)*r);
        }
        glEnd();
    }
    glPopAttrib();
}

void drawFloor(float x, float y, float z, int divs, GLuint tex, int scale)
{
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, tex);
    glNormal3f(0.f, 1.f, 0.f);
    planeDivision(x, y, z, divs, scale);
    glDisable(GL_TEXTURE_2D);
}

void drawScreen(float x, float y, float z, GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, tex);
  
    float screen_vis[] = { 1.f, 1.f, 1.f, 1.f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, screen_vis);
    
    glBegin(GL_TRIANGLES);
  
    glNormal3f(0.f, 1.f, 0.f);
    glTexCoord2f(0., 0.);
    glVertex3f(-x, y, -z);
    glTexCoord2f(1., 0.);
    glVertex3f(x, y, -z);
    glTexCoord2f(0., 1.);
    glVertex3f(-x, y, z);
    
    glTexCoord2f(0., 1.);
    glVertex3f(-x, y, z);
    glTexCoord2f(1., 0.);
    glVertex3f(x, y, -z);
    glTexCoord2f(1., 1.);
    glVertex3f(x, y, z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
   

    //Provides a bevel around plane so it merges into any surface
    glBegin(GL_QUADS);
    //Right
    glNormal3f(1.f, 0.f, 0.f);
    glVertex3f(x, y, -z);
    glVertex3f(x, y-0.1f, -z);
    glVertex3f(x, y-0.1f, z);
    glVertex3f(x, y, z);
    
    //Near
    glNormal3f(0.f, 0.f, -1.f);
    glVertex3f(-x, y, -z);
    glVertex3f(-x, y-0.1f, -z);
    glVertex3f(x, y-0.1f, -z);
    glVertex3f(x, y, -z);
    
    //Left
    glNormal3f(-1.f, 0.f, 0.f);
    glVertex3f(-x, y, z);
    glVertex3f(-x, y-0.1f, z);
    glVertex3f(-x, y-0.1f, -z);
    glVertex3f(-x, y, -z);
    
    //Back
    glNormal3f(0.f, 0.f, 1.f);
    glVertex3f(-x, y, z);
    glVertex3f(x, y, z);
    glVertex3f(x, y-0.1f, z);
    glVertex3f(-x, y-0.1f, z);
    
    glEnd();

}

void drawSkydome(float s, int depth, GLuint tex) {
        glEnable(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBindTexture(GL_TEXTURE_2D, tex);
        glDisable(GL_LIGHTING);
        
        int flag = 0;   //Flag that is ultimately passed to drawComplexTriangle function.
        const float top[3] = { 0.f, s, 0.f };
        const float bottom[3] = { 0.f, -s, 0.f };
        const float mid[4][3] = { {s, 0.f, 0.f}, {0.f, 0.f, -s}, {-s, 0.f, 0.f}, {0.f, 0.f, s} };
        float* newPoints[3];

        newPoints[0] = const_cast<float*>(top);
        newPoints[1] = const_cast<float*>(mid[0]);
        newPoints[2] = const_cast<float*>(mid[1]);
    subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[1]);
        newPoints[2] = const_cast<float*>(mid[2]);
    subDivideComplex(newPoints, depth - 1, flag);
    
        newPoints[1] = const_cast<float*>(mid[2]);
        newPoints[2] = const_cast<float*>(mid[3]);
    subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[3]);
        newPoints[2] = const_cast<float*>(mid[0]);
    subDivideComplex(newPoints, depth - 1, flag);

        newPoints[0] = const_cast<float*>(bottom);
        newPoints[1] = const_cast<float*>(mid[3]);
        newPoints[2] = const_cast<float*>(mid[2]);
    subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[2]);
        newPoints[2] = const_cast<float*>(mid[1]);
    subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[1]);
        newPoints[2] = const_cast<float*>(mid[0]);
    subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[0]);
        newPoints[2] = const_cast<float*>(mid[3]);
    subDivideComplex(newPoints, depth - 1, flag);
    
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    }

void drawSphere(float s, int depth, int mat) {
        
    if (mat == 3)
    {   //Glass specific settings
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask( GL_FALSE );
    }
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[mat][0]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[mat][1]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[mat][2]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[mat][3]);
        glMaterialfv(GL_FRONT, GL_EMISSION, materials[mat][4] );
    
        int flag = 1;   //Flag ultimately passed to draw function
        const float top[3] = { 0.f, s, 0.f };
        const float bottom[3] = { 0.f, -s, 0.f };
        const float mid[4][3] = { {s, 0.f, 0.f}, {0.f, 0.f, -s}, {-s, 0.f, 0.f}, {0.f, 0.f, s} };
        float* newPoints[3];

        // Top Half
        newPoints[0] = const_cast<float*>(top);
        newPoints[1] = const_cast<float*>(mid[0]);
        newPoints[2] = const_cast<float*>(mid[1]);
        subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[1]);
        newPoints[2] = const_cast<float*>(mid[2]);
        subDivideComplex(newPoints, depth - 1, flag);
    
        newPoints[1] = const_cast<float*>(mid[2]);
        newPoints[2] = const_cast<float*>(mid[3]);
        subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[3]);
        newPoints[2] = const_cast<float*>(mid[0]);
        subDivideComplex(newPoints, depth - 1, flag);

        //Bottom Half
        newPoints[0] = const_cast<float*>(bottom);
        newPoints[1] = const_cast<float*>(mid[3]);
        newPoints[2] = const_cast<float*>(mid[2]);
        subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[2]);
        newPoints[2] = const_cast<float*>(mid[1]);
        subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[1]);
        newPoints[2] = const_cast<float*>(mid[0]);
        subDivideComplex(newPoints, depth - 1, flag);
        
        newPoints[1] = const_cast<float*>(mid[0]);
        newPoints[2] = const_cast<float*>(mid[3]);
        subDivideComplex(newPoints, depth - 1, flag);
    
        glPopAttrib();
    
    if (mat == 3)
        {
        glDepthMask( GL_TRUE );
        glDisable( GL_BLEND );
        }
    }

void drawDisc(float size, int mat)
{   //Only used for starfield, so paramenters are deliberately limited.
    GLfloat twicePi = 2.0f * 3.141592;
    int slices = 15;
    glm::vec3 pos = glm::vec3{0.f, 0.f, 0.f};
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials[mat][0]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials[mat][1]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials[mat][2]);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, materials[mat][3]);
    glMaterialfv(GL_FRONT, GL_EMISSION, materials[mat][4] );
    
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f,1.0f, 0.0f);
    glVertex3f(pos[0], pos[1], pos[2]);
    for(int i = 0; i <= slices; i++) {
        glVertex3f(pos[0] + (size * cos(i *  twicePi / slices)), pos[1], pos[2] + ((size * sin(i * twicePi / slices))));
    }
    glEnd();
    glPopAttrib();
}

void drawStarfield(glm::vec4 starPos, glm::vec3 pos)
{
    GLfloat pi = 3.141592;
    //Calculates the x and z rotations from the stars position to the camera
    float playerZAngleRads = atan2(starPos[0] - pos[0], starPos[1]-pos[1]);
    float zRot = 180 - (playerZAngleRads * (180.0/pi));
    float playerXAngleRads = atan2(starPos[2] - pos[2], starPos[1]-pos[1]);
    float xRot = 180 + (playerXAngleRads * (180.0/pi));
    glPushMatrix();
        glTranslatef(starPos[0], starPos[1], starPos[2]);
        //Rotates view so star is always facing camera in z axis
        glRotatef(zRot, 0.f, 0.f, 1.f);
        drawDisc(starPos[3], 8);
        //Rotates view so star is always facing camera in x axis
        glRotatef(xRot, 1.f, 0.f, 0.f);
        drawDisc(starPos[3], 8);
    glPopMatrix();
 
}

