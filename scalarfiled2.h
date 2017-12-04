#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "array2.h"
#include <vector>
#include <QImage>

class ScalarField2 : public Array2
{
public:
    ScalarField2(){};
    ScalarField2(Vector2 a, Vector2 b, int ii, int jj, float defaut=0.0);
    bool load(QImage & im, Vector2 a, Vector2 b, float za, float zb);
    int inside(const Vector3 & v3);

    std::vector<float> field;

};

#endif // HEIGHTFIELD_H
