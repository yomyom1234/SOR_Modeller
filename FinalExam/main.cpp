//
//  main.cpp
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/02.
//
#define GL_SILENCE_DEPRECATION
#include "Header.h"

int moving;
Point mousebegin;
Point eye;
Point eyeLookAt;
GLuint displayList_B;
vector<model> models;
GLfloat angleY;
float scalefactor;
int scaling;
int status;
int culling;
string fname;
int modelNum;
int viewing;

void InitVariables()
{
    angleY = 0;
    scalefactor = 1.0;
    scaling = 0;
    status = 0;
    fname = "/Users/yomyom/Desktop/data/";
    modelNum = 0;
    eye.x = 0.0;
    eye.y = 300.0;
    eye.z = 500.0;
    eyeLookAt.x = 0.0;
    eyeLookAt.y = 0.0;
    eyeLookAt.z = 0.0;
    culling = FRONT_AND_BACK;
    viewing = PERSPECTIVE;
}

void InitLight()
{
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { 200, 200, -200.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MyReshape(int w,int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    GLfloat wfactor = w;
    GLfloat hfactor = h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (viewing == ORTHO)
        glOrtho(-1.0 * wfactor, 1.0 * wfactor, -1.0 * hfactor, 1.0 * hfactor, -5000.0, 5000.0);
    else if(viewing == PERSPECTIVE)
        gluPerspective(70, wfactor / hfactor, 1, 5000);
}

void GLSetupRC(void)
{
    glEnable(GL_DEPTH_TEST);/* Setup the view and projection */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z,  // eye poisition
              eyeLookAt.x, eyeLookAt.y, eyeLookAt.z,      // center is at (0,0,0)
              0.0, 1.0, 0.0);      // up is in positive Y direction
}

void display(void)
{
    int i = 0;
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    MakeBackground();
    DrawBackground();
    while (i < modelNum)
    {
        if(status == WIRE)
        {
            DrawWire(i);
            MakeGL_Model(i);
        }
        else
        {
            DrawShade(i);
            MakeGL_Model(i);
        }
        i++;
    }
    
    MyReshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
    
    //RotateY(&eye);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye.x, eye.y, eye.z, eyeLookAt.x, eyeLookAt.y, eyeLookAt.z, 0.0, 1.0, 0.0);
    angleY = 0;
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
        case'l':
            status = WIRE;
            glutPostRedisplay();
            break;
        case'f':
            status = SHADE;
            glutPostRedisplay();
            break;
        case'b':
            culling = BACK;
            glutPostRedisplay();
            break;
        case'v':
            culling = FRONT_AND_BACK;
            glutPostRedisplay();
            break;
        case'w':
            eyeLookAt.y += 5;
            glutPostRedisplay();
            break;
        case's':
            eyeLookAt.y -= 5;
            glutPostRedisplay();
            break;
        case'a':
            rotateLookAt(5);
            glutPostRedisplay();
            break;
        case'd':
            rotateLookAt(-5);
            glutPostRedisplay();
            break;
        case'q':
            forward(5.0);
            glutPostRedisplay();
            break;
        case'e':
            forward(-5.0);
            glutPostRedisplay();
            break;
        case'o':
            viewing = ORTHO;
            glutPostRedisplay();
            break;
        case'p':
            viewing = PERSPECTIVE;
            glutPostRedisplay();
            break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if(button== GLUT_LEFT_BUTTON&& state== GLUT_DOWN)
    {
        moving = 1;
        mousebegin.x = x;
    }
    if(button== GLUT_LEFT_BUTTON&& state== GLUT_UP)
        moving = 0;
    if(button== GLUT_RIGHT_BUTTON&& state== GLUT_DOWN)
    {
        scaling = 1;
        mousebegin.x = x;
    }
    if(button== GLUT_RIGHT_BUTTON&& state== GLUT_UP)
        scaling = 0;
}

void motion(int x, int y)
{
    if(moving)
    {
        angleY = angleY + (x - mousebegin.x);
        mousebegin.x = x;
        RotateY();
        glutPostRedisplay();
    }
}

void Clear()
{
    int i;
    
    i = 0;
    while (i < modelNum)
    {
        glDeleteLists(models[modelNum - i].displayList, 1);
        models.pop_back();
        i++;
    }
    modelNum = 0;
}

void Menu(int entryID)
{
    if (entryID == 1)
    {
        ReadModel();
    }
    else if (entryID == 2)
    {
        Clear();
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutCreateWindow("Dongwook Jeong");
    InitVariables();
    InitLight();
    //디스플레이
    displayList_B = glGenLists(1);
    glutDisplayFunc(display);
    
    //키보드 입력
    glutKeyboardFunc(keyboard);
    
    //마우스 입력
    glutMouseFunc(mouse);
    
    //각도, 크기 조절
    glutMotionFunc(motion);
    
    //창 크기 조절
    glutReshapeFunc(MyReshape);
    
    //메뉴
    GLint MainMenuID = glutCreateMenu(Menu);
    glutAddMenuEntry("Add Model", 1);
    glutAddMenuEntry("Clear", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    //조명
    GLSetupRC();
    glutMainLoop();
    return(0);
}
