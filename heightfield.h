#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include "geometry.h"
#include <vector>
#include <QImage>

class HeightField
{
public:
    HeightField(){};
    HeightField(Vector2 a, Vector2 b, int ii, int jj, float defaut=0.0);
    const Vector2 & getU(){return u;}
    const Vector2 & getV(){return v;}
    bool load(QImage & im, Vector2 a, Vector2 b, float za, float zb);
    int pos(int i, int j){return (i*w+j);}

    std::vector<float> field;

    int h;
    int w;
private:
    Vector2 u;
    Vector2 v;
};

#endif // HEIGHTFIELD_H
