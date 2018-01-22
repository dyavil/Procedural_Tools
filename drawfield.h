#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <vector>
#include <GL/gl.h>
#include "heightfield.h"
#include "vegetationfield.h"
#include "include/tiny_obj_loader.h"

class DrawField
{
public:
    DrawField();
    void prepare();
    void addRivers(const ScalarField2 & sf);
    void addVeget(vegetationField & sf);
    void draw();
    void prepareInterpol(int size);
    void setField(ScalarField2 sf) {fields = sf;}
    void loadTreeObj(QString path);
    ScalarField2 fields;
    std::vector<Vector3> colors;
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
    std::vector<Vector3> treeVertices;
    std::vector<Vector3> treeColors;


private:
    bool testPoint(const Vector3 & v3, int size);
};

#endif // DRAWFIELD_H
