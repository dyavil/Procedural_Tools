#include "array2.h"

Array2::Array2(Vector2 a, Vector2 b, int ww, int hh) : Box2(a, b)
{  
    h=hh;
    w=ww;
}


Vector2 Array2::get(int i, int j) {
    return Vector2((b.x-a.x)*j/(w-1), (b.y-a.y)*i/(h-1))+a;
}


Vector2 Array2::getCenter() {

    std::cout << w << std::endl;

    return Vector2(a.x + (b.x-a.x)/2.0, a.y + (b.y-a.y)/2.0);
}


std::pair<int, int> Array2::inside(const Vector3 &v3) {
    if(v3.x < a.x || v3.x > b.x || v3.y < a.y || v3.y > b.y ) return std::make_pair(-1, -1);
    Vector3 tt= v3 - Vector3(a, 0.0);
    int j = floor((tt.x)/((b.x-a.x)/(w-1)));
    int i = floor((tt.y)/((b.y-a.y)/(h-1)));
   // std::cout << v3 << " inside : " << i << ", " << j << std::endl;
    return std::make_pair(i, j);
}
