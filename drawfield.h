#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <vector>
#include <GL/gl.h>
#include "heightfield.h"

class DrawField
{
public:
    DrawField();
    void prepare();
    void addRivers(const ScalarField2 & sf);
    void addVeget(ScalarField2 & sf);
    void draw();
    void prepareInterpol(int size);
    void setField(ScalarField2 sf) {fields = sf;}
    ScalarField2 fields;
    std::vector<Vector3> colors;
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;

private:
    bool testPoint(const Vector3 & v3, int size);
};

#endif // DRAWFIELD_H
