#ifndef LAYERFIELD2_H
#define LAYERFIELD2_H

#include "heightfield.h"

class LayerField : public Array2
{
public:
    LayerField() {}
    LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1);
    LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1, ScalarField2 c2);
    LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1, ScalarField2 c2, ScalarField2 c3);

    double height(int i, int j);

    void setVegetField(ScalarField2 & veget);

    ScalarField2 couche1;
    ScalarField2 couche2;
    ScalarField2 couche3;
    ScalarField2 vegetation;
};

#endif // LAYERFIELD2_H
