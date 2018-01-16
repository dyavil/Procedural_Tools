#include "layerfield.h"

LayerField::LayerField(HeightField c1) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
    couche3 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
}

LayerField::LayerField(HeightField c1, ScalarField2 c2) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = c2;
    couche3 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
}

LayerField::LayerField(HeightField c1, ScalarField2 c2, ScalarField2 c3) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = c2;
    couche3 = c3;
}

HeightField LayerField::computeHeight(int nbCouches) {
    HeightField res(couche1.a, couche1.b, couche1.w, couche1.h);

    switch(nbCouches) {
        case 1:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)];
                }
            }
        case 2:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)] + couche2.field[pos(i, j)];
                }
            }
        case 3:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)] + couche2.field[pos(i, j)] + couche3.field[pos(i, j)];
                }
            }
            break;
        default:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = 0;
                }
            }
            break;
    }

    return res;
}


ScalarField2 LayerField::generateThemralStress(ScalarField2 & illumField, double eroMax) {
    ScalarField2 illumFieldNorm = illumField.sfNormalize();
    ScalarField2 res = ScalarField2(a, b, w, h);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.field[pos(i, j)] = std::max(0.0, eroMax * illumFieldNorm.field[pos(i, j)] - couche2.field[pos(i,j)]);
        }
    }

    return res;
}


ScalarField2 LayerField::sedimentTransport(ScalarField2 & sedField, double angleMin) {
    return ScalarField2();
}


ScalarField2 LayerField::generateThemralErosion(ScalarField2 & illumField, int nbSimu) {
    return ScalarField2();
}

void LayerField::setVegetField(ScalarField2 &veget){
    vegetation = veget;
}
