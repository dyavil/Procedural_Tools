#ifndef ARRAY2_H
#define ARRAY2_H
#include "geometry.h"
#include <utility>

class Array2 : public Box2
{
public:
    Array2(){}
    Array2(Vector2 a, Vector2 b, int ii, int jj);

    int pos(int i, int j){return (i*w+j);}
    Vector2 get(int i, int j);
    std::pair<int, int> inside(const Vector3 & v3);
    int h;
    int w;
};

#endif // ARRAY2_H
