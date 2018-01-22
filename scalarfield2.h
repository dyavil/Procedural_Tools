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
    ScalarField2() {}
    ScalarField2(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);

    bool load(QString path, double zmin, double zmax);
    void noiseMap(int pas);

    /**
     * @brief sfNormalize Normalise le scalarField entre 0 et 1
     * @return scalarField normalisé
     */
    ScalarField2 sfNormalize() const;

    QImage render();
    Vector2 gradient(int i, int j);
    void setVal(int i, int j, double val) {field[pos(i, j)] = val;}
    void CalcUV(const Vector2 & p , int & xi, int & yi, double & u, double & v);
    void Bilineaire(const Vector2 & p, double & res);
    void Barycentrique(const Vector2 & p, double & res);


    std::vector<double> field;
};

#endif // SCALARFIELD_H
