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
    void drawInterpol();
    void prepareInterpol(int size, int idf=0);
    void addField(ScalarField2 & sf){fields.push_back(sf);}
    std::vector<ScalarField2> fields;
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;

private:
    bool testPoint(const Vector3 & v3, int size, int idf);
};

#endif // DRAWFIELD_H
