//  ========================================================================
//  COSC363: Computer Graphics (2019) Assignment 1;  University of Canterbury.
//
//  FILE NAME: assignment.cpp
//  Made by: Kelly Esther Chan Ling Ing
//  Student ID: 81910282
//
//
//  Program displays a scene of a fortress with robots patrolling and a spaceship hidden behind the fort walls.
//  Use left/right arrow keys to rotate the scene
//  Use up/down arrow keys to move forward and backward
//  Use z/x keys to move up and down
//  ========================================================================
 
#include <iostream>
#include <fstream>
#include <climits>
#include <cmath>
#include <GL/freeglut.h>
#include "loadTGA.h"

using namespace std;

GLuint texId[9];


//--Globals ---------------------------------------------------------------
float *x, *y, *z;  //vertex coordinate arrays
int *t1, *t2, *t3; //triangles
int nvrt, ntri;    //total number of vertices and triangles
float angle = -90.0;  //Rotation angle for viewing
float cam_hgt = 400;

float x1 = 0;
int count = 0;

int fire = 0;
float theta = 0;

float swing = 0;

int take_off = 0;
int fly = 0;
float lookangle =20;

float look_x = 0;
float look_z= -1;

float look_y = 200;

float eye_x = 850;
float eye_z = 0;

float count_up = 1;

float robot_move = 0;

int flash_light = 0;

int spaceshipview = 0;

float teapot_rotate = 0;

int teapot_x = -200;
int teapot_y = 0;
int teapot_z = -150;
int teapot_direction = 0;
int teapot_angle = 0;

const int N = 50;

float vx[N] = {0, 8, 8, 7.5, 6.7, 5, 5.5, 4, 3.9, 3.8, 3.7, 3.8, 3.9, 4, 4.1, 4.3, 4.5, 5, 5.3, 5.5,
             5.8, 6.0, 6.2, 6.1, 6, 5.9, 5.5, 7.0, 7.3, 7.6, 8.0, 7.6, 7.3, 9.0, 9.5, 10.5,11, 11.5, 12, 12.5,
              12.8, 12.5, 11.8, 7.6, 8.5, 7, 6.1, 5.3, 4.7, 4.5};
float vy[N] = {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
               19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
               39, 40, 41, 42, 43, 43, 42, 41, 40, 39};
float vz[N] = {0};

//float equation = (-0.01 * alpha * alpha + 2 * alpha );


void normal(float x1, float y1, float z1,
            float x2, float y2, float z2,
              float x3, float y3, float z3 )
{
      float nx, ny, nz;
      nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
      ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
      nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(-nx, -ny, -nz);
}



void loadGLTextures()				// Load bitmaps And Convert To Textures
{

    glGenTextures(9, texId); 		// Create texture ids
    // *** skybox back ***
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    loadTGA("iceflats_bk.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** skybox right ***
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    loadTGA("iceflats_rt.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** skybox front ***
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    loadTGA("iceflats_ft.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** skybox left***
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    loadTGA("iceflats_lf.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // *** skybox top ***
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    loadTGA("iceflats_up.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    // *** fortress wall texture ***
    glBindTexture(GL_TEXTURE_2D, texId[5]);  //Use this texture
    loadTGA("Wall.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    // *** fortress gate texture ***
    glBindTexture(GL_TEXTURE_2D, texId[6]);  //Use this texture
    loadTGA("Gate.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    // *** vase texture ***
    glBindTexture(GL_TEXTURE_2D, texId[7]);		//Use this texture
    loadTGA("Nebula.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // *** teapot texture ***
    glBindTexture(GL_TEXTURE_2D, texId[8]);		//Use this texture
    loadTGA("Gold.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
}

void skybox(){

    glEnable(GL_TEXTURE_2D);

    ////////////////////// LEFT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000,  0, 1000);
    glTexCoord2f(1.0,0.0);
    glVertex3f(-1000, 0., -1000);
    glTexCoord2f(1.0,1.0);
    glVertex3f(-1000, 1000., -1000);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();

    ////////////////////// FRONT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000,  0, -1000);
    glTexCoord2f(1.0,0.0);
    glVertex3f(1000, 0., -1000);
    glTexCoord2f(1.0,1.0);
    glVertex3f(1000, 1000, -1000);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-1000,  1000, -1000);
    glEnd();

    ////////////////////// RIGHT WALL ///////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(1000,  0, -1000);
    glTexCoord2f(1.0,0.0);
    glVertex3f(1000, 0, 1000);
    glTexCoord2f(1.0,1.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(0.0,1.0);
    glVertex3f(1000,  1000,  -1000);
    glEnd();


    ////////////////////// REAR WALL ////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f( 1000, 0, 1000);
    glTexCoord2f(1.0,0.0);
    glVertex3f(-1000, 0,  1000);
    glTexCoord2f(1.0,1.0);
    glVertex3f(-1000, 1000,  1000);
    glTexCoord2f(0.0,1.0);
    glVertex3f( 1000, 1000, 1000);
    glEnd();

    /////////////////////// TOP //////////////////////////
    glBindTexture(GL_TEXTURE_2D, texId[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-1000, 1000, -1000);
    glTexCoord2f(1.0,0.0);
    glVertex3f(1000, 1000,  -1000);
    glTexCoord2f(1.0,1.0);
    glVertex3f(1000, 1000,  1000);
    glTexCoord2f(0.0,1.0);
    glVertex3f(-1000, 1000, 1000);
    glEnd();


}

//-- Loads mesh data in OFF format    -------------------------------------
void loadMeshFile(const char* fname)  
{
	ifstream fp_in;
	int num, ne;

	fp_in.open(fname, ios::in);
	if(!fp_in.is_open())
	{
		cout << "Error opening mesh file" << endl;
		exit(1);
	}

	fp_in.ignore(INT_MAX, '\n');				//ignore first line
	fp_in >> nvrt >> ntri >> ne;			    // read number of vertices, polygons, edges

    x = new float[nvrt];                        //create arrays
    y = new float[nvrt];
    z = new float[nvrt];

    t1 = new int[ntri];
    t2 = new int[ntri];
    t3 = new int[ntri];

	for(int i=0; i < nvrt; i++)                         //read vertex list 
		fp_in >> x[i] >> y[i] >> z[i];

	for(int i=0; i < ntri; i++)                         //read polygon list 
	{
		fp_in >> num >> t1[i] >> t2[i] >> t3[i];
		if(num != 3)
		{
			cout << "ERROR: Polygon with index " << i  << " is not a triangle." << endl;  //not a triangle!!
			exit(1);
		}
	}

	fp_in.close();
	cout << " File successfully read." << endl;
}

//--Function to compute the normal vector of a triangle with index tindx ----------


void normal(int tindx)
{
	float x1 = x[t1[tindx]], x2 = x[t2[tindx]], x3 = x[t3[tindx]];
	float y1 = y[t1[tindx]], y2 = y[t2[tindx]], y3 = y[t3[tindx]];
	float z1 = z[t1[tindx]], z2 = z[t2[tindx]], z3 = z[t3[tindx]];
	float nx, ny, nz;
	nx = y1*(z2-z3) + y2*(z3-z1) + y3*(z1-z2);
	ny = z1*(x2-x3) + z2*(x3-x1) + z3*(x1-x2);
	nz = x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2);
	glNormal3f(nx, ny, nz);
}


//--------draws the mesh model of the cannon----------------------------
void drawCannon()
{

    //Construct the object model here using triangles read from OFF file
	glBegin(GL_TRIANGLES);
		for(int tindx = 0; tindx < ntri; tindx++)
		{
		   normal(tindx);
		   glVertex3d(x[t1[tindx]], y[t1[tindx]], z[t1[tindx]]);
		   glVertex3d(x[t2[tindx]], y[t2[tindx]], z[t2[tindx]]);
		   glVertex3d(x[t3[tindx]], y[t3[tindx]], z[t3[tindx]]);
		}
	glEnd();

}

//----------draw a floor plane-------------------
void drawFloor()
{
	bool flag = false;

	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
    for(int x = -1000; x <= 1000; x += 20)
	{
        for(int z = -1000; z <= 1000; z += 20)
		{
            if(flag) glColor3f(0.745, 0.745, 0.972);

            else glColor3f(0.733, 0.733, 0.968);
			glVertex3f(x, 0, z);
			glVertex3f(x, 0, z+20);
			glVertex3f(x+20, 0, z+20);
			glVertex3f(x+20, 0, z);
			flag = !flag;
		}
	}
	glEnd();
}


//------------- draws the merlons for the fortress wall --------------
void merlon(void)
{
    glPushMatrix();
        glColor4f(0.5, 0.5, 0.5, 1.0);
        glTranslatef(0,5,0);
        glScalef(5, 5, 5);
        glutSolidCube(1.2);
    glPopMatrix();

}


//------------- maps a texture that is used for the fortress gate -----------------
void gate(void)
{
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 50.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(50.0, 50.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(50.0, 0.0, 0.0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}


//-------------- texture mapping to form 5 sides of a fortress wall and adding merlons-----------
void wall(void)
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);      //front
        glTexCoord2f(0.0,1.0);
        glVertex3f(0, 50.0, 0.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 0.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);

        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);      //back
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);

        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 0.0, 0.0);      //left side
        glTexCoord2f(1.0,0.0);
        glVertex3f(0.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(0.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0,50.0, 0.0);

        glTexCoord2f(0.0,0.0);
        glVertex3f(20.0, 0.0, 0.0);     //right side
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 0.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0,50.0, 80.0);
        glTexCoord2f(0.0,1.0);
        glVertex3f(20.0,50.0, 0.0);

        glTexCoord2f(0.0,0.0);
        glVertex3f(0.0, 50.0, 0.0);      //Top
        glTexCoord2f(0.0,1.0);
        glVertex3f(0.0, 50.0, 80.0);
        glTexCoord2f(1.0,1.0);
        glVertex3f(20.0, 50.0, 80.0);
        glTexCoord2f(1.0,0.0);
        glVertex3f(20.0, 50.0, 0.0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(17,48,25);
        merlon();
        glTranslatef(-14.3,0,0);
        merlon();
        glTranslatef(0,0,20);            //merlons
        merlon();
        glTranslatef(14.3,0,0);
        merlon();
        glTranslatef(0,0,20);
        merlon();
        glTranslatef(-14.3,0,0);
        merlon();
    glPopMatrix();
}



//------------------------constructing the fortress ---------------------------------
void fortress(void)
{
    glPushMatrix();
        glTranslatef(200, 0, 0);
        glRotatef(90,0,1,0);
        glScalef(4, 4, 4);
            glPushMatrix();
                glPushMatrix();                      //left post
                    glColor3f(0.894, 0.8, 0.647);
                    glTranslatef(-30, 25.5, 0);
                    glScalef(20, 50, 20);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();                      //cone for left post
                    glColor3f(0.549, 0.050, 0.152);
                    glTranslatef(-30, 50, 0);
                    glRotatef(-90,1,0,0);
                    glScalef(5, 5, 20);
                    glutSolidCone(1.7,1,50,5);
                glPopMatrix();

                glPushMatrix();                      //right post
                    glColor3f(0.894, 0.8, 0.647);
                    glTranslatef(30, 25.5, 0);
                    glRotatef(0, 1, 0, 0);
                    glScalef(20, 50, 20);
                    glutSolidCube(1);
                glPopMatrix();

                glPushMatrix();                      //cone for right post
                    glColor3f(0.549, 0.050, 0.152);
                    glTranslatef(30, 50, 0);
                    glRotatef(-90,1,0,0);
                    glScalef(5, 5, 20);
                    glutSolidCone(1.70,1,50,5);
                glPopMatrix();

                glPushMatrix();                      //gate
                    glTranslatef(24,50,0);
                    glRotatef(180,0,0,1);
                    gate();
                glPopMatrix();

            glPopMatrix();

            glPushMatrix();
                glTranslatef(40,0,10);               //front right wall
                glRotatef(134,0,1,0);
                wall();
            glPopMatrix();

            glPushMatrix();                          //middle right wall
                glTranslatef(97, 0, -45);
                glRotatef(-180,0,1,0);
                wall();
            glPopMatrix();

            glPushMatrix();                          //back right wall
                glTranslatef(96,0,-125);
                glRotatef(-134,0,1,0);
                wall();
            glPopMatrix();

            glPushMatrix();                          //back middle wall
                glTranslatef(39, 0, -181);
                glRotatef(-90,0,10,0);
                wall();
            glPopMatrix();

            glPushMatrix();                          //back left wall
                 glTranslatef(-84,0,-111);
                 glRotatef(134,0,1,0);
                 wall();
            glPopMatrix();

            glPushMatrix();                          //middle left wall
                glTranslatef(-77, 0, -46);
                glRotatef(180,0,1,0);
                wall();
            glPopMatrix();

            glPushMatrix();                          //front left wall
                glTranslatef(-26,0,-5);
                glRotatef(-134,0,1,0);
                wall();
            glPopMatrix();

    glPopMatrix();
}


//---------------------- draws the spaceship --------------------------
void spaceship(void)
{

    GLUquadric *q = gluNewQuadric();
    int value = flash_light/2;
    glPushMatrix();                                  //spaceship disk
        glTranslatef(-100, 50, 0);
        glColor3f(0.,1.0,1.0);
        glRotatef(-180,0,1,0);
        glRotatef(-90,1,0,0);
        gluDisk(q, 100, 140, 100, 100);
        gluQuadricDrawStyle(q, GLU_FILL);

    glPopMatrix();

    GLdouble eqn [4]={0.0,1.0,0.0,0.0};
    glPushMatrix();                                  //disk light 1

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(30,50,0);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);

        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                  //disk light 2

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(-225,60,0);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();



    glPushMatrix();                                  //disk light 3

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(-90,50,130);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();



    glPushMatrix();                                  //disk light 4

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(-185,50,96);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                   //disk light 5

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(-185,50,-96);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                    //disk light 6

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(-90,50,-130);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                    //disk light 7

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(0,50,-85);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);
        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                    //disk light 8

        if(value*2 == flash_light)
        {
            glColor3f(0.0,0.0,1.0);
        }

        else {
            glColor3f(1.0,1.0,1.0);
        }
        glTranslatef(0,50,85);
        glClipPlane(GL_CLIP_PLANE0,eqn);
        glEnable(GL_CLIP_PLANE0);

        gluSphere(q, 5.3f, 32, 32);
        glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glPushMatrix();                                     //spaceship base
        glTranslatef(-100, 50, 0);
        glColor3f(0.0,0.0,1.0);
        glRotatef(-180,0,1,0);
        glRotatef(-90,1,0,0);
        gluCylinder(q, 120, 40, 40, 100,
        100);
        gluQuadricDrawStyle(q, GLU_FILL);
    glPopMatrix();


    glPushMatrix();
        glColor3f(1.,1.0,1.0);
        glTranslatef(-100, 80, 0);
        glutSolidSphere(50, 80, 80);
    glPopMatrix();


    glPushMatrix();                                      //antenna
        glTranslatef(-100, 130, 0);
        glRotatef(-95, 1, 0, 0);

        glPushMatrix();
             glColor3f(0.0,1.0,1.0);
             glutSolidCone(5, 50, 50, 50);
        glPopMatrix();

        glPushMatrix();                                  //antenna light

         if(value*2 == flash_light)
         {
             glColor3f(1.0,0.0,0.0);
         }

         else {
             glColor3f(1.0,1.0,1.0);
         }
             glTranslatef(0, 0, 40);
             glutSolidSphere(10, 30, 30);
         glPopMatrix();


    glPopMatrix();

}

//----------------------- draws the robot --------------------------
void robot (void)
{
    glPushMatrix();
        glTranslatef(600, 50, 0);
        glScalef(5, 5, 5);
        GLdouble eqn [4]={0.0,1.0,0.0,0.0};
        glPushMatrix();                                   //head
            glColor3f(0.4, 0.7, 0.0);
            glTranslatef(0,9.5,0);
            glClipPlane(GL_CLIP_PLANE0,eqn);
            glEnable(GL_CLIP_PLANE0);
            GLUquadric *q = gluNewQuadric();
            gluSphere(q, 5.3f, 32, 32);
            glDisable(GL_CLIP_PLANE0);

            glPushMatrix();                                //eyes
                glColor3f(1.0, 1.0, 1.0);
                glTranslatef(2,2,4.5);
                glutSolidCube(1);
                glTranslatef(-4,0,0);
                glutSolidCube(1);
            glPopMatrix();

            glPushMatrix();                                //robot antennas
                glColor3f(0.4, 0.7, 0.0);
                glTranslatef(-5,8,0);
                glRotatef(90,1,0,0);
                glRotatef(30,0,1,0);
                gluCylinder(q,0.3,0.3,5.,30.,30);
                gluQuadricDrawStyle(q, GLU_FILL);
                glTranslatef(8,0,5);
                glRotatef(-60,0,1,0);
                gluCylinder(q,0.3,0.3,5.,30.,30);
                gluQuadricDrawStyle(q, GLU_FILL);
            glPopMatrix();

        glPopMatrix();


        glPushMatrix();                                     //neck
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(0,10,0);
            glRotatef(90,1,0,0);
            GLUquadric *c = gluNewQuadric();
            gluCylinder(c,5.0,5.0,3.,30.,30);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();

        glPushMatrix();                                      //body
            glColor3f(0.4, 0.7, 0.0);
            glTranslatef(0,9,0);
            glRotatef(90,1,0,0);
            gluCylinder(q,5.3,5.3,8.,30.,30);
            gluQuadricDrawStyle(q, GLU_FILL);
        glPopMatrix();


        glPushMatrix();                                       //robot arms
           glScalef(0.3, 0.3, 0.3);
                glPushMatrix();                               //right arm
                    glTranslatef(-21, 13, 0);
                    glTranslatef(-2, 6.5, 0);
                    glRotatef(swing, 1, 0, 0);
                    glTranslatef(2, -6.5 , 0);

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,9,0);
                        glClipPlane(GL_CLIP_PLANE0,eqn);
                        glEnable(GL_CLIP_PLANE0);
                        gluSphere(q, 5.3f, 32, 32);
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,9,0);
                        glRotatef(90,1,0,0);
                        gluCylinder(q,5.3,5.3,12.,30.,30);
                        gluQuadricDrawStyle(q, GLU_FILL);
                    glPopMatrix();

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,-2,0);
                        glRotatef(180,1,0,0);
                        glClipPlane(GL_CLIP_PLANE0,eqn);
                        glEnable(GL_CLIP_PLANE0);
                        gluSphere(q, 5.3f, 32, 32);
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();

                glPopMatrix();

                glPushMatrix();                                     //left arm
                    glTranslatef(21, 13, 0);
                    glTranslatef(2, 6.5, 0);
                    glRotatef(-swing, 1, 0, 0);
                    glTranslatef(-2, -6.5, 0);

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,9,0);
                        glClipPlane(GL_CLIP_PLANE0,eqn);
                        glEnable(GL_CLIP_PLANE0);
                        gluSphere(q, 5.3f, 32, 32);
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,9,0);
                        glRotatef(90,1,0,0);
                        gluCylinder(q,5.3,5.3,12.,30.,30);
                        gluQuadricDrawStyle(q, GLU_FILL);
                    glPopMatrix();

                    glPushMatrix();
                        glColor3f(0.4, 0.7, 0.0);
                        glTranslatef(0,-2,0);
                        glRotatef(180,1,0,0);
                        glClipPlane(GL_CLIP_PLANE0,eqn);
                        glEnable(GL_CLIP_PLANE0);
                        gluSphere(q, 5.3f, 32, 32);
                        glDisable(GL_CLIP_PLANE0);
                    glPopMatrix();

                glPopMatrix();

            glPushMatrix();                                         //robot bottom
                glColor3f(0.4, 0.7, 0.0);
                glTranslatef(0, -10, 0);
                glRotatef(-180,0,1,0);
                glRotatef(-90,1,0,0);
                gluCylinder(q, 25, 10, 35, 100,
                100);
                gluQuadricDrawStyle(q, GLU_FILL);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();

}


//----------------- draws the cannon -----------------
void cannon(void)

{
    float light[] = {0, 300, 400, 1};
    float shadowMat[16] = { 300,0,0,0, 0,0,-400,-1,
                            0,0,300,0, 0,0,0,300 };

    //***------ cannon shadow -------------
    glLightfv(GL_LIGHT0, GL_POSITION, light);
    glDisable(GL_LIGHTING);

    glPushMatrix();
        glTranslatef(300,0,0);
        glPushMatrix();
            glTranslatef(0, 1, 0);
            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20, 30, 0);
                glRotatef(30.0, 0, 0, 1);
                glTranslatef(20, -30, 0);
                glColor4f(0.2,0.2,0.2,1);
                drawCannon();
            glPopMatrix();


            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-10,5,17);
                glScalef(80,10,6);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20,25,17);
                glScalef(40,30,6);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-10,5,-17);
                glScalef(80,10,6);
                glutSolidCube(1.0);
            glPopMatrix();

            glPushMatrix();
                glMultMatrixf(shadowMat);
                glTranslatef(-20,25,-17);
                glScalef(40,30,6);
                glutSolidCube(1.0);
            glPopMatrix();


            glPushMatrix();
                glMultMatrixf(shadowMat);
                float equation = (-0.01 * x1 * x1 + 2 * x1 );

                if (equation < 0) {
                    x1 = 0;
                    count += 1;
                }
                if(count == 0) {
                    glTranslatef(38.8 + theta, 64 + equation, 0);

                }
                else {
                    glTranslatef(38.8 + theta, (equation)/ (2*(count * count)), 0);
                }

                glutSolidSphere(5, 36, 18);
            glPopMatrix();

        glPopMatrix();

    glPopMatrix();


    //***-------cannon object ---------------
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(300,0,0);
        glPushMatrix();
            glTranslatef(-20, 30, 0);
            glRotatef(30.0, 0, 0, 1);
            glTranslatef(20, -30, 0);
            glColor3f(0.4, 0.5, 0.4);
            drawCannon();
        glPopMatrix();

        glPushMatrix();
            glColor3f(0.6,0.3,0);
            glTranslatef(-10,5,17);
            glScalef(80,10,6);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-20,25,17);
            glScalef(40,30,6);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-10,5,-17);
            glScalef(80,10,6);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-20,25,-17);
            glScalef(40,30,6);
            glutSolidCube(1.0);
        glPopMatrix();

        glPushMatrix();
            glColor3f(0,0,0);

            float equation2 = (-0.01 * x1 * x1 + 2 * x1 );

            if (equation2 < 0) {
                x1 = 0;
                count += 1;
            }
            if(count == 0) {
                glTranslatef(38.8 + theta, 64 + equation2, 0);

            }
            else {
                glTranslatef(38.8 + theta, (equation2)/ (2*(count * count)), 0);
            }


            glutSolidSphere(5, 36, 18);
        glPopMatrix();

    glPopMatrix();

}

//-------------- draws the teapot -----------
void teapot (void)
{
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);                              //enable texture for teapot
        glBindTexture(GL_TEXTURE_2D, texId[8]);
        glTranslatef(-125,110,-30);
        glTranslatef(teapot_x, teapot_y, teapot_z);
        glRotatef(teapot_rotate, 0, 1, 0);
        glScalef(4,4,4);
        glutSolidTeapot(5);


        if(teapot_x == -200 && teapot_z == -150)
        {
            teapot_direction = 0;
            teapot_angle=90;
        }
        else if(teapot_x==200 && teapot_z==-150)
        {
            teapot_direction = 1;
            teapot_angle=90;
        }
        else if(teapot_x==200 && teapot_z==200)
        {
            teapot_direction = 2;
            teapot_angle=90;
        }
        else if(teapot_x==-200 && teapot_z==200)
        {
            teapot_direction = 3;
            teapot_angle=0;
        }

        if(teapot_direction == 0)
        {
           teapot_x+=5;
        }
        else if(teapot_direction == 1)
        {
           teapot_z+=5;
        }
        else if(teapot_direction == 2)
        {
           teapot_x-=5;
        }
        else if(teapot_direction == 3)
        {
           teapot_z-=5;
        }

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

//--------------- draws the vase ---------------
void vase (void)
{
    glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texId[7]);

        float wx[N], wy[N], wz[N];
        float angStep = 10.0*3.1415926/180.0;

        for(int j = 0; j < 36; j++) //36 slices in 10 deg steps
        {
        for(int i = 0; i < N; i++)
        {
            wx[i] =(vx[i] * cos(angStep)) + (vz[i] * sin(angStep));
            wy[i] = vy[i];
            wz[i] = (-vx[i] * sin(angStep)) + ( vz[i] * cos(angStep));

            glBegin(GL_TRIANGLE_STRIP);

             if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],
             vx[i-1], vy[i-1], vz[i-1],
            vx[i], vy[i], vz[i] );

             glTexCoord2f(j/36.0, i/(float)N);
             glVertex3f(vx[i], vy[i], vz[i]);

             if(i > 0) normal( wx[i-1], wy[i-1], wz[i-1],
             vx[i], vy[i], vz[i],
             wx[i], wy[i], wz[i] );

             glTexCoord2f((j+1)/36.0, i/(float)N);
             glVertex3f(wx[i], wy[i], wz[i]);
        }

        glEnd();
        for (int k = 0; k < N; k++)
           {
               vx[k] = wx[k];
               vy[k] = wy[k];
               vz[k] = wz[k];
           }


        }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


//--Display: ----------------------------------------------------------------------
//--This is the main display module containing function calls for generating
//--the scene.
void display()  
{

    float lgt_pos[] = {4000.0f, 2000.0f, -1000.0f, 1.0f};    //light0 position

    float lgt1_pos[] = {680.0f, 40.0f, 0.0f, 1.0f};  //robot light (light1)

    float spot_dir[] = {-1, -1, 0};

    glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);    //GL_LINE = Wireframe;   GL_FILL = Solid

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();


    float cdr=3.14159265/180.0;
       look_x = eye_x + 100*sin(angle*cdr);
       look_z = eye_z - 100*cos(angle*cdr);
       look_y =  cam_hgt - 100*tan(lookangle*cdr);;

    if (spaceshipview == 1 &&(take_off < 1000))
    {
        gluLookAt(0, 200+take_off, 0, 100, 200, 0, 0 , 1, 0);
    }
    else {
        gluLookAt(eye_x, cam_hgt, eye_z, look_x, look_y, look_z, 0, 1, 0);
    }

    fortress();

    cannon();

    drawFloor();

    glPushMatrix();
        glTranslatef(-100.0, 0.0, 0.0);
        glRotatef(-robot_move, 0, 1, 0);
        glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_dir);
        robot();
    glPopMatrix();


    glPushMatrix();
        glRotatef(robot_move, 0, 1, 0);
        glLightfv(GL_LIGHT2, GL_POSITION, lgt1_pos);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_dir);
        glTranslatef(1200.0, 0.0, 0.0);
        glRotatef(-180, 0, 1, 0);
        robot();
    glPopMatrix();


    glPushMatrix();

        if (fly == 1)
        {
            glTranslatef(0, take_off, 0);
        }

        glTranslatef(0, take_off, 0);
        spaceship();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-320, 0, 170);
        glScalef(2,2,2);
        vase();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(-320, 0, -170);
        glScalef(2,2,2);
        vase();
    glPopMatrix();

    glPushMatrix();
        glTranslatef(60, 0, 170);
        glScalef(2,2,2);
        vase();
    glPopMatrix();


    glPushMatrix();
        glTranslatef(60, 0, -170);
        glScalef(2,2,2);
        vase();
    glPopMatrix();

    teapot();

    skybox();

    glDisable(GL_TEXTURE_2D);

    glutSwapBuffers();

}


void keyboard(unsigned char key, int, int)
{

    if(key == 'c'|| (key == 'C')) {                     //fires the cannon
        fire = 1;
    }
    else if(key == 's' || key == 'S'){                  //fly spaceship
        fly = 1;
    }
    else if(key == 'z'|| (key == 'Z')) {                //look up
        lookangle--;
    }
    else if ((key == 'x') || (key == 'X')){             //look down
        lookangle ++;
    }
}

//------------ Special key event callback ---------------------------------

void special(int key, int x, int y)
{
    float cdr=3.14159265/180.0;

    if(key == GLUT_KEY_LEFT) {                          //left direction
        angle -= 0.8;
    }
    else if(key == GLUT_KEY_RIGHT) {                    //right direction
        angle += 0.8;
    }
    else if(key == GLUT_KEY_DOWN ) {                    //Move backward
        eye_x -= 10*sin(angle*cdr);
        eye_z += 10*cos(angle*cdr);
    }
    else if(key == GLUT_KEY_UP) {                       // Move forward
        eye_x += 10*sin(angle*cdr);
        eye_z -= 10*cos(angle*cdr);
    }

     else if(key == GLUT_KEY_HOME) {                    //switch to spaceship camera
        if(spaceshipview == 0) {
         spaceshipview = 1;
        }
        else {
            spaceshipview = 0;
        }
    }

    look_x = eye_x + 100*sin(angle*cdr);
    look_z = eye_z - 100*cos(angle*cdr);

    //----- collision detection (stay within the skybox) ---------
    if(eye_x >= 850) {
        eye_x = 850;
    }
    else if(eye_x <= -850) {
        eye_x =- 850;
    }
    if(eye_z >= 850) {
        eye_z = 850;
    }
    else if(eye_z <= -850) {
        eye_z =- 850;
    }

    glutPostRedisplay();

}


//------- timer for teapot ----------
void teapot_timer(int value)
{
    teapot_rotate ++;
    if(teapot_rotate > 360) {
        teapot_rotate = 0;
    }

    glutTimerFunc(10, teapot_timer, value);
    glutPostRedisplay();

}


//---------timer for robot's arms ----------
void robot_timer(int value)
{
    if (swing > 20) {
        count_up = -1;
    }
    if (swing < -20) {
        count_up = 1;
    }
    swing += count_up;

    glutTimerFunc(1, robot_timer, 0);
    glutPostRedisplay();
}

//--------timer for robot to move around fortress ----------
void robot_walk_timer(int value)
{
    robot_move ++;
    if(robot_move > 360) {
        robot_move = 0;
    }

    glutTimerFunc(20, robot_walk_timer, value);
    glutPostRedisplay();

}

//--------timer for flashing lights -----
void flash_timer (int value)
{
    flash_light ++;
    glutTimerFunc(20, flash_timer, value);
    //glutPostRedisplay();

}


//--------timer for cannonball ------
void cannon_timer(int value)
{
    if(fire == 1 && theta <= 600 ) {
        theta += 2;
        x1 += 2;
    }
    else {
        fire = 0;
        theta = 0;
        x1 = 0;
        count = 0;
    }
    glutTimerFunc(1, cannon_timer, 0);
    glutPostRedisplay();

}

//-------timer for spaceship to take off ------
void spaceship_timer(int value)
{
    if(fly == 1) {
        take_off ++;
    }

    glutTimerFunc(1, spaceship_timer, 0);
    glutPostRedisplay();

}


//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{

    float grey[4] = {0.2, 0.2, 0.2, 1.0};
    float white[4]  = {1.0, 1.0, 1.0, 1.0};

    loadGLTextures();

    glEnable(GL_TEXTURE_2D);
    loadMeshFile("assignment.off");				//Specify mesh file name here
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);       //Background colour

    glEnable(GL_LIGHTING);                      //Enable OpenGL states
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
 	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT1, GL_AMBIENT, grey);        //robot light 1
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, white);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);

    glLightfv(GL_LIGHT2, GL_AMBIENT, grey);        //robot light 2
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,0.01);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(80.0, 1.0, 100.0, 5000.0);


}


//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (800, 800);
    glutInitWindowPosition (10, 10);
    glutCreateWindow ("Assignment");
    initialize();
    glutTimerFunc(20, cannon_timer, 0);
    glutTimerFunc(1, spaceship_timer, 0);
    glutTimerFunc(1, robot_timer, 0);
    glutTimerFunc(10, robot_walk_timer, 0);
    glutTimerFunc(20, flash_timer, 0);
    glutTimerFunc(10, teapot_timer, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMainLoop();
    return 0;

}
