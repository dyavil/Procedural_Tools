#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <fstream>
#include "scalarfield2.h"
#include "vector"

class HeightField : public ScalarField2
{
public:
    HeightField();
    HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);
    Vector3 normal(int i, int j);
    double slope(int i, int j);

    ScalarField2 generateSlopeField();
    void exportOBJ(const std::string & filename, bool importNormals = true);


};

#endif // HEIGHTFIELD_H
