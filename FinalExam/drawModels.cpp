//
//  drawModels.cpp
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/07.
//
#define GL_SILENCE_DEPRECATION

#include "Header.h"

void DrawWire(int i)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (culling == 0)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
        glDisable(GL_CULL_FACE);
    glCallList(models[i].displayList);
}

void DrawShade(int i)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (culling == 0)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }
    else
        glDisable(GL_CULL_FACE);
    glCallList(models[i].displayList);
}

void DrawBackground(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCallList(displayList_B);
}
