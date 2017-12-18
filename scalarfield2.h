#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include "array2.h"
#include <vector>
#include <QImage>
#include <time.h>
#include <stdlib.h>

class ScalarField2 : public Array2
{
public:
    ScalarField2(){};
    ScalarField2(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);
    bool load(QImage & im, Vector2 a, Vector2 b, double za, double zb);
    void noiseMap(int pas);
    QImage render();
    Vector2 gradient(int i, int j);
    void setVal(int i, int j, double val){field[pos(i, j)] = val;}
    void CalcUV(const Vector2 & p , int & xi, int & yi, double & u, double & v);
    void Bilineaire(const Vector2 & p, double & res);
    void Barycentrique(const Vector2 & p, double & res);


    std::vector<double> field;

};

#endif // SCALARFIELD_H
