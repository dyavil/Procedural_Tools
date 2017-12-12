#include "scalarfield2.h"

ScalarField2::ScalarField2(Vector2 a, Vector2 b, int ii, int jj, double defaut) : Array2(a, b, ii, jj)
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
            int ii = im.height()-i-1;
            field[pos(ii, j)] = zb-(((zb-za)*clrC.black())/(255));
        }
    }

    return true;
}


QImage ScalarField2::render(){
    QImage res = QImage(w, h, QImage::Format_RGB32);
    QRgb val;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int ii = h-i-1;
            int v = floor(field[pos(i, j)] * 255);
            val = qRgb(v, v, v);
            res.setPixel(j, ii, val);
        }
    }
    return res;
}


Vector2 ScalarField2::gradient(int i, int j){
    double aa = 2.0*((b.x-a.x)/w);
    double dx;
    if(j+1 >= w) dx = (field[pos(i, j)] - field[pos(i, j-1)])/(aa/2.0);
    if(j-1 < 0) dx = (field[pos(i, j+1)] - field[pos(i, j)])/(aa/2.0);
    else dx = (field[pos(i, j+1)] - field[pos(i, j-1)])/aa;
    aa = 2*((b.y-a.y)/h);
    double dy;
    if(i+1 >= h) dy = (field[pos(i, j)] - field[pos(i-1, j)])/(aa/2.0);
    if(i-1 < 0) dy = (field[pos(i+1, j)] - field[pos(i, j)])/(aa/2.0);
    else dy = (field[pos(i+1, j)] - field[pos(i-1, j)])/aa;
    return Vector2(dx, dy);
}
