#include "scalarfiled2.h"

ScalarField2::ScalarField2(Vector2 a, Vector2 b, int ii, int jj, float defaut) : Array2(a, b, ii, jj)
{
    field.resize(ii*jj);
    for (int i = 0; i < jj; ++i) {
        for (int j = 0; j < ii; ++j) {
             field[i*ii+j] = defaut;
        }
    }

}


bool ScalarField2::load(QImage & im, Vector2 a, Vector2 b, float za, float zb){
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


int ScalarField2::inside(const Vector3 &v3){
    if(v3.x < a.x || v3.x > b.x || v3.y < a.y || v3.y > b.y ) return -1;
    int j = (v3.x)/((b.x-a.x)/w);
    int i = (v3.y)/((b.y-a.y)/h);
    std::cout << i << ", " << j << std::endl;
    return pos(i, j);
}
