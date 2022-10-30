//
//  SOR_Modeller.hpp
//  SOR_Modeller
//
//  Created by 정동욱 on 2022/10/21.
//

#ifndef SOR_Modeller_h
#define SOR_Modeller_h

#include <stdio.h>
#include <math.h>
class xpoint3D
{
    public:
    GLint x;
    GLint y;
    GLint w;
    GLint z;
    xpoint3D()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 1;
    }
    xpoint3D(const xpoint3D& newpoint)
    {
        x = newpoint.x;
        y = newpoint.y;
        z = newpoint.z;
        w = newpoint.w;
    }
};

extern std::vector<std::vector<xpoint3D>> arPoints_Array;
extern int NUM_POINT;
extern int ANGLE;
extern int POINTSHAPE;

void ClearArr();

#endif /* SOR_Modeller_h */
