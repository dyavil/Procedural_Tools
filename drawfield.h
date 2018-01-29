#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <vector>
#include <GL/gl.h>
#include "heightfield.h"
#include "layerfield.h"
#include "vegetationfield.h"
#include "include/tiny_obj_loader.h"

class DrawField
{
public:
    DrawField();
    void prepare();
    void addRivers(const ScalarField2 & sf);

    void addVeget(vegetationField & sf);
    void draw(bool showTree);
    void prepareInterpol(int size);
    void setField(HeightField sf) {fields = sf;}
    void loadTreeObj(QString path, int pos = 0);
    HeightField fields;
    std::vector<Vector3> colors;
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
    std::vector<std::vector<Vector3>> treeVertices;
    std::vector<std::vector<Vector3>> treeColors;
    std::vector<std::vector<Vector3>> treeTranslations;
    std::vector<double> larg;


private:
    bool testPoint(const Vector3 & v3, int size);
    int idStartTree;
};

#endif // DRAWFIELD_H
