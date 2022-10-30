//
//  main.cpp
//  SOR_Modeller
//
//  Created by 정동욱 on 2022/10/20.
//

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "SOR_Modeller.hpp"
#include "ShapeHandler.hpp"

std::vector<std::vector<xpoint3D>> arPoints_Array;
int NUM_POINT = 0;
int ANGLE = 360;
int MODE = 0;
int POINTSHAPE = 0;
int IS_FULL = 0;


void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1000, 700);
    glColor3f(0.0, 0.0, 0.0);

    //x, y축 생성
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-500.0, 0.0, 0.0);
    glVertex3f(500.0, 0.0, 0.0);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0, 350.0, 0.0);
    glVertex3f(0.0, -350.0, 0.0);
    glEnd();
    
    //점찍기
    int rot_num = 360 / ANGLE;
    if (MODE == 0)
    {
        glBegin(GL_POINTS);
        for(int i = 0; i < rot_num; i++)
        {
            for(int j = 0; j < NUM_POINT; j++)
            {
                MakePoint(i, j);
            }
        }
        glEnd();
    }
    else if (MODE == 2)
    {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        MakeTriangle();
        glEnd();
        glBegin(GL_POINTS);
        for(int i = 0; i < rot_num; i++)
        {
            for(int j = 0; j < NUM_POINT; j++)
            {
                MakePoint(i, j);
            }
        }
        glEnd();
    }
    else if (MODE == 3)
    {
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        MakePolygon();
        glEnd();
        glBegin(GL_POINTS);
        for(int i = 0; i < rot_num; i++)
        {
            for(int j = 0; j < NUM_POINT; j++)
            {
                MakePoint(i, j);
            }
        }
        glEnd();
    }
    glutSwapBuffers();
    glFlush();
}


//마우스로 점 입력받기
void Mouse(GLint button, GLint action, GLint x, GLint y)
{
    if (!IS_FULL)
    {
        xpoint3D newpoint;
        if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
        {
            newpoint.x = 1 * (x - 500);
            newpoint.y = -1 * (y - 350);
            arPoints_Array[0].push_back(newpoint);
            NUM_POINT++;
        }
    }
    else
    {
        if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
        {
            ClearArr();
            IS_FULL = 0;
        }
    }
    glutPostRedisplay();
}

void ClearArr()
{
    for (int i = 0; i < (360 / ANGLE); i++)
    {
        arPoints_Array[i].clear();
    }
    arPoints_Array.clear();
    NUM_POINT = 0;
    ANGLE = 360;
    std::vector<xpoint3D> defaultPoints;
    arPoints_Array.push_back(defaultPoints);
    glutPostRedisplay();
}



//메뉴
void Menu(int entryID)
{
    int angle;
    if (entryID == 1)
    {
        std::cout << "Input Degree" << std::endl;
        scanf("%d", &angle);
        if (360 % angle == 0)
        {
            ANGLE = angle;
            RotateX();
            IS_FULL = 1;
        }
        else
        {
            std::cout << "angle must devided by 360 " << std::endl;
        }
    }
    else if (entryID == 2)
    {
        std::cout << "Input Degree" << std::endl;
        scanf("%d", &angle);
        if (360 % angle == 0)
        {
            ANGLE = angle;
            RotateY();
            IS_FULL = 1;
        }
        else
        {
            std::cout << "angle must devided by 360 " << std::endl;
        }
    }
    else if (entryID == 3)
    {
        if (NUM_POINT)
        {
            arPoints_Array[0].pop_back();
            NUM_POINT -= 1;
        }
        glutPostRedisplay();
    }
    else if (entryID == 4)
    {
        ClearArr();
    }
}

void ModeSubMenu(int entryID)
{
    if (entryID == 3)
    {
        //triangles
        MODE = 2;
        glutPostRedisplay();
    }
    else if (entryID == 4)
    {
        //polygon
        MODE = 3;
        glutPostRedisplay();
    }
}

void PointSubMenu(int entryID)
{
    if (entryID == 1)
    {
        MODE = 0;
        POINTSHAPE = 0;
        glutPostRedisplay();
    }
    else if (entryID == 2)
    {
        MODE = 0;
        POINTSHAPE = 1;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    std::vector<xpoint3D> defaultPoints;
    arPoints_Array.push_back(defaultPoints);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1000,700);
    glutInitWindowPosition(0,0);
    glutCreateWindow("SOR_Modeller_DongWook");
    glClearColor(0.7,0.7,0.7,1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-500.0,500.0,-350.0,350.0,-500.0,500.0);
    
    //콜백
    glutMouseFunc(Mouse);
    glutDisplayFunc(Display);
    
    //메뉴
    GLint PointSubMenuID = glutCreateMenu(PointSubMenu);
    glutAddMenuEntry("Default", 1);
    glutAddMenuEntry("Cat", 2);
    GLint ModeSubMenuID = glutCreateMenu(ModeSubMenu);
    glutAddSubMenu("Points", PointSubMenuID);
    glutAddMenuEntry("Triangles", 3);
    glutAddMenuEntry("Polygon", 4);
    GLint MainMenuID = glutCreateMenu(Menu);
    glutAddMenuEntry("Rotate X", 1);
    glutAddMenuEntry("Rotate Y", 2);
    glutAddSubMenu("Change Mode", ModeSubMenuID);
    glutAddMenuEntry("Cancel", 3);
    glutAddMenuEntry("Clear", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    glutMainLoop();
    return (0);
}
