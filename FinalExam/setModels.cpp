//
//  setModels.cpp
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/06.
//
#define GL_SILENCE_DEPRECATION

#include "Header.h"
#include <random>

void find_boundary(model* newmodel)
{
    int i = 0;
    float max[3] = {-5000, -5000, -5000};
    float min[3] = {5000, 5000, 5000};
    
    
    while (i < newmodel->pnum)
    {
        if (newmodel->mpoint[i].x > max[0])
            max[0] = newmodel->mpoint[i].x;
        if (newmodel->mpoint[i].y > max[1])
            max[1] = newmodel->mpoint[i].y;
        if (newmodel->mpoint[i].z > max[2])
            max[2] = newmodel->mpoint[i].z;
        if (newmodel->mpoint[i].x < min[0])
            min[0] = newmodel->mpoint[i].x;
        if (newmodel->mpoint[i].y < min[1])
            min[1] = newmodel->mpoint[i].y;
        if (newmodel->mpoint[i].z < min[2])
            min[2] = newmodel->mpoint[i].z;
        i++;
    }
    newmodel->max[0] = max[0] + newmodel->translateFactor.x;
    newmodel->max[1] = max[1] + newmodel->translateFactor.y;
    newmodel->max[2] = max[2] + newmodel->translateFactor.z;
    newmodel->min[0] = min[0] + newmodel->translateFactor.x;
    newmodel->min[1] = min[1] + newmodel->translateFactor.y;
    newmodel->min[2] = min[2] + newmodel->translateFactor.z;
}

int ReadModel()
{
    FILE* f1;
    char s[81];
    int i;
    char modelname[100];
    model* newmodel = new model;
    Point* newTranslfactor = new Point;

    printf("Input Model Name\n");
    scanf("%s", modelname);
    string modellocation = fname + modelname +".dat";
    if((f1 = fopen(modellocation.c_str(), "rt")) == NULL)
    {
        printf("No file\n");
        return (-1);
    }
    fscanf(f1, "%s", s);
    //printf("%s", s);
    fscanf(f1, "%s", s);
    //printf("%s", s);
    fscanf(f1, "%d", &(newmodel->pnum));
    //printf("%d\n", (newmodel->pnum));
    newmodel->mpoint = new Point[(newmodel->pnum)];
    for (i = 0; i < (newmodel->pnum); i++)
    {
        fscanf(f1, "%f", &newmodel->mpoint[i].x);
        fscanf(f1, "%f", &newmodel->mpoint[i].y);
        fscanf(f1, "%f", &newmodel->mpoint[i].z);
    }
    fscanf(f1, "%s", s);
    //printf("%s", s);
    fscanf(f1, "%s", s);
    //printf("%s", s);
    fscanf(f1, "%d", &(newmodel->fnum));
    //printf("%d\n", (newmodel->fnum));
    newmodel->mface = new Face[(newmodel->fnum)];
    for(i = 0; i < (newmodel->fnum); i++)
    {
        fscanf(f1, "%d", &newmodel->mface[i].ip[0]);
        fscanf(f1, "%d", &newmodel->mface[i].ip[1]);
        fscanf(f1, "%d", &newmodel->mface[i].ip[2]);
    }
    fclose(f1);
    newmodel->displayList = glGenLists(1);
    
    
    //Location
    printf("Input Model Location");
    printf("X :\n");
    scanf("%f", &(newTranslfactor->x));
    printf("Y :\n");
    scanf("%f", &(newTranslfactor->y));
    printf("Z :\n");
    scanf("%f", &(newTranslfactor->z));
    newmodel->translateFactor = *(newTranslfactor);
    
    
    //random
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<float> dis(0, 1);
    newmodel->random_r = dis(gen);
    newmodel->random_g = dis(gen);
    newmodel->random_b = dis(gen);
    
    
    //boundary
    find_boundary(newmodel);
    
    models.push_back(*newmodel);
    modelNum++;
    printf("%s located\n", modelname);
    return (1);
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

void MakeGL_Model(int k)
{
    int i;
    
    glShadeModel(GL_SMOOTH);
    glNewList(models[k].displayList, GL_COMPILE);
    GLfloat mat_diffuse[] = { models[k].random_r, models[k].random_g, models[k].random_b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { models[k].random_r, models[k].random_g, models[k].random_b, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glPushMatrix();
    glTranslatef(models[k].translateFactor.x, models[k].translateFactor.y, models[k].translateFactor.z);
    for(i = 0; i < models[k].fnum; i++)
    {
        Point norm = cnormal(models[k].mpoint[models[k].mface[i].ip[2]], models[k].mpoint[models[k].mface[i].ip[1]], models[k].mpoint[models[k].mface[i].ip[0]]);
        glBegin(GL_TRIANGLES);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(models[k].mpoint[models[k].mface[i].ip[0]].x, models[k].mpoint[models[k].mface[i].ip[0]].y, models[k].mpoint[models[k].mface[i].ip[0]].z);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(models[k].mpoint[models[k].mface[i].ip[1]].x, models[k] .mpoint[models[k].mface[i].ip[1]].y, models[k].mpoint[models[k].mface[i].ip[1]].z);
        glNormal3f(norm.x, norm.y, norm.z);
        glVertex3f(models[k].mpoint[models[k].mface[i].ip[2]].x, models[k].mpoint[models[k].mface[i].ip[2]].y, models[k].mpoint[models[k].mface[i].ip[2]].z);
        glEnd();
    }
    glPopMatrix();
    glEndList();
}

void MakeBackground(void)
{
    glShadeModel(GL_SMOOTH);
    glNewList(displayList_B, GL_COMPILE);
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_QUADS);
    glVertex3f(700, 0, 700);
    glVertex3f(700, 0, -700);
    glVertex3f(-700, 0, -700);
    glVertex3f(-700, 0, 700);
    glEnd();
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat[]){ 1.0, 0.0, 0.0, 1.0 });
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat[]){ 1.0, 0.0, 0.0, 1.0 });
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 1, 1000);
    glVertex3f(0, 1, -1000);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 700, 0);
    glVertex3f(0, -700, 0);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex3f(1000, 1, 0);
    glVertex3f(-1000, 1, 0);
    glEnd();
    glPopMatrix();
    glEndList();
}
