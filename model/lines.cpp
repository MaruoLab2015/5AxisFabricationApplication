#include "lines.h"

#include <QDebug>
Lines::Lines()
{
}

Lines::Lines(QList<GCode*> gcodeList)
{
    float curr_x, curr_y, curr_z;
    curr_x = curr_y = curr_z = 0;
    bool curr_e = 0;

    for(int i=0;i<gcodeList.count();i++)
    {
        GCode *gcode;
        gcode = gcodeList[i];

        qglviewer::Vec *vec;
        vec = new qglviewer::Vec();


        if(gcode->hasX()) vec->x = gcode->x;
        else vec->x = curr_x;
        if(gcode->hasY()) vec->y = gcode->y;
        else vec->y = curr_y;
        if(gcode->hasZ()) vec->z = gcode->z;
        else vec->z = curr_z;

        vertices.append(vec);

        if(gcode->hasE())
            if( gcode->e == 1)
            {
                shutterFlagList.append(true);
                curr_e = true;
            }
            else
            {
                shutterFlagList.append(false);
                curr_e = false;
            }
        else
            shutterFlagList.append(curr_e);

    }
}

void Lines::computedBoudingBox()
{
    GLfloat
            min_x, max_x,
            min_y, max_y,
            min_z, max_z;

    min_x = max_x = vertices[0]->x;
    min_y = max_y = vertices[0]->y;
    min_z = max_z = vertices[0]->z;
    for (int i = 0; i < vertices.size(); i++) {
        if (vertices[i]->x < min_x) min_x = vertices[i]->x;
        if (vertices[i]->x > max_x) max_x = vertices[i]->x;
        if (vertices[i]->y < min_y) min_y = vertices[i]->y;
        if (vertices[i]->y > max_y) max_y = vertices[i]->y;
        if (vertices[i]->z < min_z) min_z = vertices[i]->z;
        if (vertices[i]->z > max_z) max_z = vertices[i]->z;
    }
    size = qglviewer::Vec(max_x-min_x, max_y-min_y, max_z-min_z);
    center = qglviewer::Vec((min_x+max_x)/2, (min_y+max_y)/2, (min_z+max_z)/2);
    min = qglviewer::Vec(min_x, min_y, min_z);
    max = qglviewer::Vec(max_x, max_y, max_z);
}
