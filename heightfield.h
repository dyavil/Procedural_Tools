#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "scalarfield2.h"
#include "vector"

class HeightField : public ScalarField2
{
public:
    HeightField();
    HeightField(Vector2 a, Vector2 b, int ii, int jj, float defaut=0.0);
    Vector3 normal(int i, int j);
    void CalcUV(const Vector2 & p , int & xi, int & yi, float & u, float & v);
    void Bilineaire(const Vector2 & p, double & res);
};

#endif // HEIGHTFIELD_H
