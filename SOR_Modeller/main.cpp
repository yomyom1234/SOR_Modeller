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

//추출
void Extract()
{
    int pnum;
    int fnum;
    int** mpoint;
    int** mface;
    char modelname[100];
    int i = 0;
    int j = 0;
    string fname = "/Users/yomyom/Desktop/data/";
    
    //점, 면, 모델 이름
    printf("Input Model Name\n");
    scanf("%s", modelname);
    string modellocation = fname + modelname +".dat";
    pnum = NUM_POINT * (360 / ANGLE);
    fnum = (NUM_POINT - 1) * 2 * (360 / ANGLE);
    
    //점 할당
    mpoint = (int **)malloc(sizeof(int *) * pnum);
    while (i < 360 / ANGLE)
    {
        j = 0;
        while (j < NUM_POINT)
        {
            mpoint[(i * NUM_POINT) + j] = new int[3];
            mpoint[(i * NUM_POINT) + j][0] = arPoints_Array[i][j].x;
            mpoint[(i * NUM_POINT) + j][1] = arPoints_Array[i][j].y;
            mpoint[(i * NUM_POINT) + j][2] = arPoints_Array[i][j].z;
            j++;
        }
        i++;
    }
    
    i = 0;
    j = 0;

    
    //면 할당
    int k = 0;
    mface = (int **)malloc(sizeof(int *) * fnum);
    while (i < (360 / ANGLE) - 1)
    {
        j = 0;
        while (j < (NUM_POINT - 1))
        {
            mface[k] = new int[3];
            mface[k][0] = (i * NUM_POINT) + j;
            mface[k][1] = (i * NUM_POINT) + j + 1;
            mface[k][2] = ((i + 1) * NUM_POINT) + j;
            k++;
            mface[k] = new int[3];
            mface[k][0] = ((i + 1) * NUM_POINT) + j;
            mface[k][1] = (i * NUM_POINT) + j + 1;
            mface[k][2] = ((i + 1) * NUM_POINT) + j + 1;
            k++;
            j++;
        }
        i++;
    }
    j = 0;
    while (j < (NUM_POINT - 1))
    {
        mface[k] = new int[3];
        mface[k][0] = (i * NUM_POINT) + j;
        mface[k][1] = (i * NUM_POINT) + j + 1;
        mface[k][2] = j;
        k++;
        mface[k] = new int[3];
        mface[k][0] = j;
        mface[k][1] = (i * NUM_POINT) + j + 1;
        mface[k][2] = j + 1;
        k++;
        j++;
    }
    
    //파일에 쓰기
    
    FILE *fout;
    
    fout = fopen(modellocation.c_str(), "w");
    fprintf(fout, "VERTEX =  %d\n", pnum);
    for (int i = 0; i < pnum; i++)
    {
        fprintf( fout, "%d %d %d\n", mpoint[i][0], mpoint[i][1], mpoint[i][2]);
    }
    fprintf(fout, "FACE = %d\n", fnum);
    for(int i = 0; i < fnum; i++)
    {
        fprintf(fout, "%d %d %d\n", mface[i][0], mface[i][1], mface[i][2]);
    }
    fclose(fout);
    printf("total_point = %d, total_faces = %d, k = %d", pnum, fnum, k);
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
    else if (entryID == 5)
    {
        Extract();
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
    glutAddMenuEntry("Extract", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    
    glutMainLoop();
    return (0);
}
