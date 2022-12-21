//
//  translateModels.cpp
//  CG_FinalExam
//
//  Created by 정동욱 on 2022/12/10.
//
#define GL_SILENCE_DEPRECATION

#include "Header.h"

int is_in_boundary()
{
    int i = 0;
    float newnewx;
    float newnewy;
    float newnewz;
    
    newnewx = eye.x - eyeLookAt.x;
    newnewy = eye.y - eyeLookAt.y;
    newnewz = eye.z - eyeLookAt.z;
    
    float newx = newnewz * sin(angleY * 0.01) + newnewx * cos(angleY * 0.01) + eyeLookAt.x;
    float newy = newnewy + eyeLookAt.y;
    float newz = newnewz * cos(angleY * 0.01) - newnewx * sin(angleY * 0.01) + eyeLookAt.z;
    
    while (i < modelNum)
    {
        if (newx >= models[i].min[0] && newx <= models[i].max[0])
        {
            if (newy >= models[i].min[1] && newy <= models[i].max[1])
            {
                if (newz >= models[i].min[2] && newz <= models[i].max[2])
                {
                    return 0;
                }
            }
        }
        i++;
    }
    return 1;
}

void RotateY(void)
{
    float newx;
    float newy;
    float newz;
    
    newx = eye.x - eyeLookAt.x;
    newy = eye.y - eyeLookAt.y;
    newz = eye.z - eyeLookAt.z;
    if (is_in_boundary() == 1)
    {
        eye.x = newz * sin(angleY * 0.01) + newx * cos(angleY * 0.01) + eyeLookAt.x;
        eye.y = newy + eyeLookAt.y;
        eye.z = newz * cos(angleY * 0.01) - newx * sin(angleY * 0.01) + eyeLookAt.z;
    }
}

void rotateLookAt(int angle)
{
    float newx;
    float newy;
    float newz;
    
    newx = eyeLookAt.x - eye.x;
    newy = eyeLookAt.y - eye.y;
    newz = eyeLookAt.z - eye.z;
    eyeLookAt.x = newz * sin(angle * 0.01) + newx * cos(angle * 0.01) + eye.x;
    eyeLookAt.y = newy + eye.y;
    eyeLookAt.z = newz * cos(angle * 0.01) - newx * sin(angle * 0.01) + eye.z;
}

int is_in_boundary(Point viewVector, int degree)
{
    int i = 0;
    float newx = eye.x + viewVector.x * degree;
    float newy = eye.y + viewVector.y * degree;
    float newz = eye.z + viewVector.z * degree;
    
    while (i < modelNum)
    {
        if (newx >= models[i].min[0] && newx <= models[i].max[0])
        {
            if (newy >= models[i].min[1] && newy <= models[i].max[1])
            {
                if (newz >= models[i].min[2] && newz <= models[i].max[2])
                {
                    return 0;
                }
            }
        }
        i++;
    }
    return 1;
}

void forward(float degree)
{
    Point viewVector;
    float newx;
    float newy;
    float newz;
    float distance;
    
    //거리 벡터
    viewVector.x = eyeLookAt.x - eye.x;
    viewVector.y = eyeLookAt.y - eye.y;
    viewVector.z = eyeLookAt.z - eye.z;
    
    //값 저장
    newx = viewVector.x;
    newy = viewVector.y;
    newz = viewVector.z;
    
    //거리 구해서 정규화
    distance = pow((pow(newx, 2) + pow(newy, 2) + pow(newz, 2)), 0.5);
    viewVector.x = newx / distance;
    viewVector.y = newy / distance;
    viewVector.z = newz / distance;
    
    //정규화한거 인자만큼 더하기
    if (is_in_boundary(viewVector, degree) == 1)
    {
        eye.x += viewVector.x * degree;
        eye.y += viewVector.y * degree;
        eye.z += viewVector.z * degree;
        eyeLookAt.x += viewVector.x * degree;
        eyeLookAt.y += viewVector.y * degree;
        eyeLookAt.z += viewVector.z * degree;
    }
}
