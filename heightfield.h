#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <fstream>
#include <unordered_map>
#include "scalarfield2.h"
#include "vector"

class HeightField : public ScalarField2
{
public:
    HeightField() {}
    HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);

    Vector3 normalOld(int i, int j);
    Vector3 normal(int i, int j);
    void CalcUV(const Vector2 & p , int & xi, int & yi, float & u, float & v);
    void Bilineaire(const Vector2 & p, double & res);
    void Barycentrique(const Vector2 & p, double & res);
    double slope(int i, int j);
    ScalarField2 generateSlopeField();
    void exportOBJ(const std::string & filename, bool importNormals = true);
};

#endif // HEIGHTFIELD_H
