#include "scalarfield2.h"

ScalarField2::ScalarField2(Vector2 a, Vector2 b, int ii, int jj, float defaut) : Array2(a, b, ii, jj)
{
    field.resize(ii*jj);
    for (int i = 0; i < jj*ii; ++i) {
             field[i] = defaut;
    }

}


bool ScalarField2::load(QImage & im, Vector2 a, Vector2 b, double za, double zb){
    this->a=a;
    this->b=b;
    h=im.height();
    w=im.width();
    field.resize(im.width()*im.height());
    if(im.isNull()) return false;

    for (int i = 0; i < im.height(); ++i) {
        for (int j = 0; j < im.width(); ++j) {
            QColor clrC( im.pixel( j, i ) );
            field[pos(i, j)] = zb-(((zb-za)*clrC.black())/(255));
        }
    }

    return true;
}

