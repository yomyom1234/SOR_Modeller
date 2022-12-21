//
//  test.cpp
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/02.
//

#define GL_SILENCE_DEPRECATION
#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <math.h>

using namespace std;

#define WIRE 0
#define SHADE 1

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

GLfloat angle = 0;
float scalefactor= 1.0;
int fnum;
Point* mpoint = NULL;
Face* mface = NULL;
int pnum;
int scaling = 0;
int moving;
int mousebegin;
int status = 0;           // WIRE or SHADE
string fname= "/Users/yomyom/Desktop/data/chair.dat";
GLuint displayList;
GLuint ortho_left = -1.0;
GLuint ortho_right = 1.0;
GLuint ortho_top = 1.0;
GLuint ortho_bottom = -1.0;
GLuint ortho_near = -5000.0;
GLuint ortho_far = 5000.0;

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
    glShadeModel(GL_SMOOTH);  glEnable(GL_LIGHTING);
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

Point cnormal(Point a, Point b, Point c)
{
    Point p, q, r;
    double val;
    p.x= a.x-b.x;
    p.y= a.y-b.y;
    p.z= a.z-b.z;
    q.x= c.x-b.x;
    q.y= c.y-b.y;
    q.z= c.z-b.z;
    r.x= p.y* q.z-p.z* q.y;
    r.y= p.z* q.x-p.x* q.z;
    r.z= p.x* q.y-p.y* q.x;
    val= sqrt(r.x* r.x+ r.y* r.y+ r.z* r.z);
    r.x= r.x/ val;
    r.y= r.y/ val;
    r.z= r.z/ val;
    return (r);
}

void MyReshape(int w,int h)
{
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ortho_left *= w;
    ortho_right *= w;
    ortho_top *= h;
    ortho_bottom *= h;
    glOrtho(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);
}

void MakeGL_Model(void)
{
    int i;
    glShadeModel(GL_SMOOTH);
    if(glIsList(1))
        glDeleteLists(1, 1);
    glNewList(displayList + 1, GL_COMPILE);
    glPushMatrix();
    glColor3f(1, 0, 0);
    for(i = 0; i < fnum; i++)
    {
        Point norm = cnormal(mpoint[mface[i].ip[2]], mpoint[mface[i].ip[1]], mpoint[mface[i].ip[0]]);
        glBegin(GL_TRIANGLES);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(mpoint[mface[i].ip[0]].x, mpoint[mface[i].ip[0]].y, mpoint[mface[i].ip[0]].z);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(mpoint[mface[i].ip[1]].x, mpoint[mface[i].ip[1]].y, mpoint[mface[i].ip[1]].z);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(mpoint[mface[i].ip[2]].x, mpoint[mface[i].ip[2]].y, mpoint[mface[i].ip[2]].z);
        glEnd();
    }
    glPopMatrix();
    glEndList();
}

void MakeBackground(void)
{
    glShadeModel(GL_SMOOTH);
    glNewList(displayList, GL_COMPILE);
    glPushMatrix();
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(500, 0, 500);
    glVertex3f(500, 0, -500);
    glVertex3f(-500, 0, -500);
    glVertex3f(-500, 0, 500);
    glEnd();
    glPopMatrix();
    glEndList();
}

void GLSetupRC(void)
{
    glEnable(GL_DEPTH_TEST);/* Setup the view and projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, // field of view in degree
                   1.0, // aspect ratio
                   1.0, // Z near
                   2000.0); // Z far
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 500.0, 500.0,  // eye poisition
              0.0, 0.0, 0.0,      // center is at (0,0,0)
              0.0, 1.0, 0.);      // up is in positive Y direction
}

void DrawWire(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glCallList(displayList + 1);
    glutSwapBuffers();
}

void DrawShade(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(displayList + 1);
    glutSwapBuffers();
}

void DrawBackground(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(displayList);
}

void display(void)
{
    displayList =glGenLists(10);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    MakeBackground();
    DrawBackground();
    MakeGL_Model();
    if(status == WIRE)
        DrawWire();
    else
        DrawShade();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    ortho_left *= scalefactor;
    ortho_right *= scalefactor;
    ortho_top *= scalefactor;
    ortho_bottom *= scalefactor;
    glOrtho(ortho_left, ortho_right, ortho_bottom, ortho_top, ortho_near, ortho_far);
}

void ReadModel()
{
    FILE* f1;
    char s[81];
    int i;
    if (mpoint != NULL)
        delete mpoint;
    if(mface != NULL)
        delete mface;
    if((f1 = fopen(fname.c_str(), "rt")) == NULL)
    {
        printf("No file\n");
        exit(0);
        
    }
    fscanf(f1, "%s", s);
    printf("%s", s);
    fscanf(f1, "%s", s);
    printf("%s", s);
    fscanf(f1, "%d", &pnum);
    printf("%d\n", pnum);
    mpoint = new Point[pnum];
    for (i = 0; i < pnum; i++)
    {
        fscanf(f1, "%f", &mpoint[i].x);
        fscanf(f1, "%f", &mpoint[i].y);
        fscanf(f1, "%f", &mpoint[i].z);
        printf("%f %f %f\n", mpoint[i].x, mpoint[i].y, mpoint[i].z);
        
    }
    fscanf(f1, "%s", s);
    printf("%s", s);
    fscanf(f1, "%s", s);
    printf("%s", s);
    fscanf(f1, "%d", &fnum);
    printf("%d\n", fnum);
    mface = new Face[fnum];
    for(i = 0; i < fnum; i++)
    {
        fscanf(f1, "%d", &mface[i].ip[0]);
        fscanf(f1, "%d", &mface[i].ip[1]);
        fscanf(f1, "%d", &mface[i].ip[2]);
        printf("%d %d %d\n", mface[i].ip[0], mface[i].ip[1], mface[i].ip[2]);
        
    }
    fclose(f1);
}

void keyboard(unsigned char key, int x, int y)
{
    printf("key %d\n", key);
    switch(key)
    {
        case'w':
            status = WIRE;
            glutPostRedisplay();
            break;
        case's':
            status = SHADE;
            glutPostRedisplay();
            break;
        case'1':
            fname="/Users/yomyom/Desktop/data/cube.dat";
            ReadModel();
            glutPostRedisplay();
            break;
        case'2':
            fname="/Users/yomyom/Desktop/data/mysphere.dat";
            ReadModel();
            glutPostRedisplay();
            break;
        case'3':
            fname="/Users/yomyom/Desktop/data/myellipsoid.dat";
            ReadModel();
            glutPostRedisplay();
            break;
        case'4':
            fname="/Users/yomyom/Desktop/data/mytorus.dat";
            ReadModel();
            glutPostRedisplay();
            break;
        case'5':
            fname="/Users/yomyom/Desktop/data/mybspline.dat";
            ReadModel();
            glutPostRedisplay();
            break;
    }
}

void mouse(int button, int state, int x, int y)
{
    if(button== GLUT_LEFT_BUTTON&& state== GLUT_DOWN)
    {
        moving = 1;
        mousebegin= x;
    }
    if(button== GLUT_LEFT_BUTTON&& state== GLUT_UP)
        moving = 0;
    if(button== GLUT_RIGHT_BUTTON&& state== GLUT_DOWN)
    {
        scaling = 1;
        mousebegin= x;
    }
    if(button== GLUT_RIGHT_BUTTON&& state== GLUT_UP)
        scaling = 0;
}

void motion(int x, int y)
{
    if(scaling)
    {
        scalefactor= scalefactor* (1.0 + (mousebegin-x) * 0.000001);
        glutPostRedisplay();
    }
    if(moving)
    {
        angle = angle + (x-mousebegin);
        mousebegin= x;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);
    glutCreateWindow("Dongwook Jeong");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(MyReshape);
    ReadModel();
    GLSetupRC();
    InitLight();
    glutMainLoop();
    return(0);
}
