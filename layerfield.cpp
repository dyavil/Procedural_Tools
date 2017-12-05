#include "layerfield.h"

LayerField::LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1) : Array2(a, b, nx, ny) {
    couche1 = c1;
    couche2 = ScalarField2(a, b ,nx, ny);
    couche3 = ScalarField2(a, b ,nx, ny);
}

LayerField::LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1, ScalarField2 c2) : Array2(a, b, nx, ny) {
    couche1 = c1;
    couche2 = c2;
    couche3 = ScalarField2(a, b ,nx, ny);
}

LayerField::LayerField(Vector2 a, Vector2 b, int nx, int ny, ScalarField2 c1, ScalarField2 c2, ScalarField2 c3) : Array2(a, b, nx, ny) {
    couche1 = c1;
    couche2 = c2;
    couche3 = c3;
}
