#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <vector>
#include <GL/gl.h>
#include "heightfield.h"

class DrawField
{
public:
    DrawField();
    void draw();
    void drawInterpol(int rapport, int idf=0);
    void addField(ScalarField2 & sf){fields.push_back(sf);}
    std::vector<ScalarField2> fields;
};

#endif // DRAWFIELD_H
