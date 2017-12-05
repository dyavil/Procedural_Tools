#include "array2.h"

Array2::Array2(Vector2 a, Vector2 b, int ii, int jj) : Box2(a, b)
{  
    h=jj;
    w=ii;
}


Vector2 Array2::get(int i, int j){

    return Vector2((b.x-a.x)*j/w, (b.y-a.y)*i/h)+a;
}


std::pair<int, int> Array2::inside(const Vector3 &v3){
    if(v3.x < a.x || v3.x > b.x || v3.y < a.y || v3.y > b.y ) return std::make_pair(-1, -1);
    Vector3 tt= v3 - Vector3(a, 0.0);
    int j = floor((tt.x)/((b.x-a.x)/w));
    int i = floor((tt.y)/((b.y-a.y)/h));
    return std::make_pair(i, j);
}
