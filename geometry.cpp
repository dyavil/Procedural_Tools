#include "geometry.h"

bool Box2::inBox(const Vector2 & p){
    if((p.y < b.y) && (p.y > a.y) && (p.x < b.x) && (p.x > a.x)) return true;
    return false;
}

bool Box3::inBox(const Vector3 & p){
    float d = (pmin-Vector3(pmin.x, pmin.y, pmax.z)).length();
    float w = ( pmin-Vector3(pmax.x, pmin.y, pmin.z)).length();
    float h = ( pmin-Vector3(pmin.x, pmax.y, pmin.z)).length();
    if (p.x >= pmin.x
        && p.x < pmin.x + w
        && p.y >= pmin.y
        && p.y < pmin.y + h
        && p.z >= pmin.z
        && p.z < pmin.z + d)
           return true;
       else
           return false;
    return false;
}


bool Box3::hitTest(const Box3 &b3){
    float d = (pmin-Vector3(pmin.x, pmin.y, pmax.z)).length();
    float w = ( pmin-Vector3(pmax.x, pmin.y, pmin.z)).length();
    float h = ( pmin-Vector3(pmin.x, pmax.y, pmin.z)).length();

    float od = (b3.pmin-Vector3(b3.pmin.x, b3.pmin.y, b3.pmax.z)).length();
    float ow = ( b3.pmin-Vector3(b3.pmax.x, b3.pmin.y, b3.pmin.z)).length();
    float oh = ( b3.pmin-Vector3(b3.pmin.x, b3.pmax.y, b3.pmin.z)).length();

    if((b3.pmin.x >= pmin.x + w)
        || (b3.pmin.x + ow <= pmin.x)
        || (b3.pmin.y >= pmin.y + h)
        || (b3.pmin.y + oh <= pmin.y)
            || (b3.pmin.z >= pmin.z + d)
        || (b3.pmin.z + od <= pmin.z))
              return false;
       else
              return true;
}
