#include "array2.h"

Array2::Array2(Vector2 a, Vector2 b, int ii, int jj) : Box2(a, b)
{  
    h=jj;
    w=ii;
}



Vector2 Array2::get(int i, int j){

    return Vector2((b.x-a.x)*j/w, (b.y-a.y)*i/h)+a;
}
