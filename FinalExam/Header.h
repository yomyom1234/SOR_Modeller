//
//  Header.h
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/06.
//

#ifndef Header_h
#define Header_h

#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <string>
#include <math.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <cmath>

#define GL_SILENCE_DEPRECATION
#define WIRE 0
#define SHADE 1
#define BACK 0
#define FRONT_AND_BACK 1
#define ORTHO 0
#define PERSPECTIVE 1

using namespace std;

//struct
typedef struct
{
    float x;
    float y;
    float z;
} Point;

typedef struct
{
    unsigned int ip[3];
} Face;

typedef struct
{
    Point* mpoint;
    Face* mface;
    GLuint displayList;
    int fnum;
    int pnum;
    Point translateFactor;
    
    GLfloat random_r;
    GLfloat random_g;
    GLfloat random_b;
    
    float max[3];
    float min[3];
} model;

//functions
int ReadModel();
void MakeGL_Model(int k);
void MakeBackground(void);
void DrawWire(int i);
void DrawShade(int i);
void DrawBackground();
void RotateY();
void rotateLookAt(int angle);
void forward(float degree);


//Variables
extern string fname;
extern vector<model> models;
extern int modelNum;
extern GLuint displayList_B;
extern GLfloat angleY;
extern Point eyeLookAt;
extern Point eye;
extern int culling;

#endif /* Header_h */
