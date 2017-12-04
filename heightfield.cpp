#include "heightfield.h"

HeightField::HeightField(Vector2 a, Vector2 b, int ii, int jj, float defaut)
{
    u=a;
    v=b;
    h=jj;
    w=ii;
    field.resize(ii*jj);
    for (int i = 0; i < jj; ++i) {
        for (int j = 0; j < ii; ++j) {
             field[i*ii+j] = defaut;
        }
    }

}


bool HeightField::load(QImage & im, Vector2 a, Vector2 b, float za, float zb){
    u=a;
    v=b;
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
