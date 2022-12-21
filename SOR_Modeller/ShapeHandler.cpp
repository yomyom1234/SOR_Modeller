//
//  ShapeHander.cpp
//  SOR_Modeller
//
//  Created by 정동욱 on 2022/10/21.
//
#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <vector>
#include "SOR_Modeller.hpp"
#include "ShapeHandler.hpp"

void vert(int arr[][2], int i, int j, int n)
{
    for (int k = 0; k < n; k++)
    {
        glVertex3f(arPoints_Array[i][j].x + arr[k][0], arPoints_Array[i][j].y + arr[k][1], arPoints_Array[i][j].z);
    }
}

void MakePoint(int i, int j)
{
    if (POINTSHAPE == 0)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(1.0);
        int red[][2] = {{0,0}, {-1,0}, {0,-1}, {0,-2}, {0,-3}, {0,1}, {0,2}, {-1,-1}, {-1,-2}, {-1,-3}, {-1,1}, {-1,2}, {-1,-1}, {1,1}, {1,0}, {1,-1}, {1,-2}, {-2,1}, {-2,0}, {-2,-1}, {-2,-2}, {-3,0}, {-3,-1}, {2,0}, {2,-1}};
        vert(red, i, j, 25);

        glColor3f(0.0, 0.0, 1.0);
        int blue[][2] = {{3,0}, {3,-1}, {-4,0}, {-4,-1}, {2,1}, {2,2}, {-3,1}, {-3,2}, {2,-2}, {2,-3}, {-3,-2}, {1,2}, {-2,2}, {1,-3}, {-2,-3}, {-3,-3}, {0,-4}, {0,3}, {-1,-4}, {-1,3}};
        vert(blue, i, j, 20);
    }
    else if (POINTSHAPE == 1)
    {
        glPointSize(1.0);
        glColor3f(0.0, 0.0, 0.0);
        int black[][2] = {{0,0}, {-2,1}, {2,1}, {-3,-3}, {-2,-3}, {-1,-3}, {0,-3}, {1,-3}, {2,-3}, {3,-3}, {-4,-2}, {4,-2}, {-5,-1}, {5,-1}, {-6,0}, {-6,1}, {-6,2}, {-6,3}, {6,0}, {6,1}, {6,2}, {6,3}, {-5,4}, {5,4}, {-6,5}, {-6,6}, {-6,7}, {-6,8}, {6,5}, {6,6}, {6,7}, {6,8}, {-5,9}, {5,9}, {-4,8}, {4,8}, {-3,7}, {-2,7}, {-1,7}, {0,7}, {1,7}, {2,7}, {3,7}};
        vert(black, i, j, 43);
        glColor3f(1.0, 1.0, 1.0);
        int white[][2] = {{5,8}, {4,7}, {5,7}, {5,6}, {5,5}, {1,6}, {2,6}, {3,6}, {1,5}, {2,5}, {3,5}, {0,4}, {1,4}, {2,4}, {3,4}, {4,4}, {-1,3}, {0,3}, {1,3}, {2,3}, {3,3}, {4,3}, {5,3}, {-3,2}, {-2,2}, {-1,2}, {0,2}, {1,2}, {2,2}, {3,2}, {-5,1}, {-4,1}, {-3,1}, {-1,1}, {0,1}, {1,1}, {3,1}, {4,1}, {5,1}, {-2,0}, {-1,0}, {1,0}, {2,0}, {-4,-1}, {-3,-1}, {-2,-1}, {-1,-1}, {0,-1}, {1,-1}, {2,-1}, {3,-1}, {4,-1}, {-3,-2}, {-2,-2}, {-1,-2}, {0,-2}, {1,-2}, {2,-2}, {3,-2}};
        vert(white, i, j, 59);
        glColor3f(0.9, 0.24, 0.24);
        int pink[][2] = {{-3,0}, {3,0}};
        vert(pink, i, j, 2);
        glColor3f(1.0, 0.85, 0.54);
        int yellow[][2] = {{-4,6}, {-4,5}, {4,6}, {4,5}};
        vert(yellow, i, j, 4);
        glColor3f(0.78, 0.55, 0.32);
        int lightbrown[][2] = {{-5,2}, {-4,2}, {-5,0}, {-4,0}, {4,2}, {5,2}, {4,0}, {5,0}};
        vert(lightbrown, i, j, 8);
        glColor3f(0.51, 0.36, 0.21);
        int brown[][2] = {{-5,8}, {-5,7}, {-4,7}, {-5,6}, {-3,6}, {-2,6}, {-1,6}, {0,6}, {-5,5}, {-3,5}, {-2,5}, {-1,5}, {0,5}, {-4,4}, {-3,4}, {-2,4}, {-1,4}, {-5,3}, {-4,3}, {-3,3}, {-2,3}};
        vert(brown, i, j, 21);
    }
}

void RotateY()
{
    int rot_num = 360 / ANGLE;
    float Radian = ANGLE * (M_PI / 180);
    for(int i=0; i < rot_num - 1; i++)
    {
        std::vector<xpoint3D> arPoints;
        for(int j=0; j < NUM_POINT; j++)
        {
            xpoint3D newpoint;
            newpoint.x = arPoints_Array[0][j].z * sin(Radian * (i + 1)) + arPoints_Array[0][j].x * cos(Radian * (i + 1));
            newpoint.y = arPoints_Array[0][j].y;
            newpoint.z = arPoints_Array[0][j].z * cos(Radian * (i + 1)) - arPoints_Array[0][j].x * sin(Radian * (i + 1));
            arPoints.push_back(newpoint);
        }
        arPoints_Array.push_back(arPoints);
    }
    glutPostRedisplay();
}

void RotateX()
{
    int rot_num = 360 / ANGLE;
    float Radian = ANGLE * (M_PI / 180);
    for(int i=0; i < rot_num - 1; i++)
    {
        std::vector<xpoint3D> arPoints;
        for(int j=0; j < NUM_POINT; j++)
        {
            xpoint3D newpoint;
            newpoint.x = arPoints_Array[0][j].x;
            newpoint.y = arPoints_Array[0][j].y * cos(Radian * (i + 1)) - arPoints_Array[0][j].z * sin(Radian * (i + 1));
            newpoint.z = arPoints_Array[0][j].y * sin(Radian * (i + 1)) + arPoints_Array[0][j].z * cos(Radian * (i + 1));
            arPoints.push_back(newpoint);
        }
        arPoints_Array.push_back(arPoints);
    }
    glutPostRedisplay();
}

void MakeTriangle()
{
    int rot_num = 360 / ANGLE;
    for(int i = 0; i < rot_num; i++)
    {
        for(int k = 0; k < NUM_POINT - 1; k++)
        {
            glVertex3f(arPoints_Array[i][k].x, arPoints_Array[i][k].y, arPoints_Array[i][k].z);
            glVertex3f(arPoints_Array[i][k + 1].x, arPoints_Array[i][k + 1].y, arPoints_Array[i][k + 1].z);
        }
        for(int j = 0; j < NUM_POINT; j++)
        {
            if (i != 0)
            {
                glVertex3f(arPoints_Array[i][j].x, arPoints_Array[i][j].y, arPoints_Array[i][j].z);
                glVertex3f(arPoints_Array[i - 1][j].x, arPoints_Array[i - 1][j].y, arPoints_Array[i - 1][j].z);
            }
            else
            {
                glVertex3f(arPoints_Array[0][j].x, arPoints_Array[0][j].y, arPoints_Array[0][j].z);
                glVertex3f(arPoints_Array[rot_num - 1][j].x, arPoints_Array[rot_num - 1][j].y, arPoints_Array[rot_num - 1][j].z);
            }
        }
        for(int j = 0; j < NUM_POINT - 1; j++)
        {
            if (i != rot_num - 1)
            {
                glVertex3f(arPoints_Array[i][j].x, arPoints_Array[i][j].y, arPoints_Array[i][j].z);
                glVertex3f(arPoints_Array[i + 1][j + 1].x, arPoints_Array[i + 1][j + 1].y, arPoints_Array[i + 1][j + 1].z);
            }
            else
            {
                glVertex3f(arPoints_Array[0][j].x, arPoints_Array[0][j].y, arPoints_Array[0][j].z);
                glVertex3f(arPoints_Array[rot_num - 1][j + 1].x, arPoints_Array[rot_num - 1][j + 1].y, arPoints_Array[rot_num - 1][j + 1].z);
            }
        }
        for(int j = 1; j < NUM_POINT; j++)
        {
            if (i != rot_num - 1)
            {
                glVertex3f(arPoints_Array[i][j].x, arPoints_Array[i][j].y, arPoints_Array[i][j].z);
                glVertex3f(arPoints_Array[i + 1][j - 1].x, arPoints_Array[i + 1][j - 1].y, arPoints_Array[i + 1][j - 1].z);
            }
            else
            {
                glVertex3f(arPoints_Array[0][j].x, arPoints_Array[0][j].y, arPoints_Array[0][j].z);
                glVertex3f(arPoints_Array[rot_num - 1][j - 1].x, arPoints_Array[rot_num - 1][j - 1].y, arPoints_Array[rot_num - 1][j - 1].z);
            }
        }
    }
}

void MakePolygon()
{
    int rot_num = 360 / ANGLE;
    for(int i = 0; i < rot_num; i++)
    {
        for(int k = 0; k < NUM_POINT - 1; k++)
        {
            glVertex3f(arPoints_Array[i][k].x, arPoints_Array[i][k].y, arPoints_Array[i][k].z);
            glVertex3f(arPoints_Array[i][k + 1].x, arPoints_Array[i][k + 1].y, arPoints_Array[i][k + 1].z);
        }
        for(int j = 0; j < NUM_POINT; j++)
        {
            if (i != 0)
            {
                glVertex3f(arPoints_Array[i][j].x, arPoints_Array[i][j].y, arPoints_Array[i][j].z);
                glVertex3f(arPoints_Array[i - 1][j].x, arPoints_Array[i - 1][j].y, arPoints_Array[i - 1][j].z);
            }
            else
            {
                glVertex3f(arPoints_Array[0][j].x, arPoints_Array[0][j].y, arPoints_Array[0][j].z);
                glVertex3f(arPoints_Array[rot_num - 1][j].x, arPoints_Array[rot_num - 1][j].y, arPoints_Array[rot_num - 1][j].z);
            }
        }
    }
}
