#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include "array2.h"
#include <vector>
#include <QImage>

class ScalarField2 : public Array2
{
public:
    ScalarField2(){};
    ScalarField2(Vector2 a, Vector2 b, int ii, int jj, float defaut=0.0);
    bool load(QImage & im, Vector2 a, Vector2 b, double za, double zb);
    void setVal(int i, int j, double val){field[pos(i, j)] = val;}
    std::vector<double> field;

};

#endif // SCALARFIELD_H
