#include "heightfield.h"

HeightField::HeightField()
{

}


HeightField::HeightField(Vector2 a, Vector2 b, int ii, int jj, float defaut) : ScalarField2(a, b, ii, jj, defaut){}

Vector3 HeightField::normal(int i, int j){
    Vector3 va, vb, vc, vd;
    if((i+1) < h) va = Vector3(get(i+1, j), field[pos(i+1, j)]) - Vector3(get(i, j), field[pos(i, j)]);
    if((j+1) < w) vb = Vector3(get(i, j+1), field[pos(i, j+1)]) - Vector3(get(i, j), field[pos(i, j)]);
    if((i-1) >= 0) vc = Vector3(get(i-1, j), field[pos(i-1, j)]) - Vector3(get(i, j), field[pos(i, j)]);
    if((j-1) >= 0) vd = Vector3(get(i, j-1), field[pos(i, j-1)]) - Vector3(get(i, j), field[pos(i, j)]);
    Vector3 r, r2, r3, r4;
    int n = 4;
    if((i+1) < h && (j+1) < w) r = cross(vb, va);
    else n--;
    if((j+1) < w && (i-1) >= 0) r2 = cross(vc, vb);
    else n--;
    if((i-1) >= 0 && (j-1) >= 0)r3 = cross(vd, vc);
    else n--;
    if((i+1) < h && (j-1) >= 0) r4 = cross(va, vd);
    else n--;
    return normalize((r+r2+r3+r4)/n);
}


void HeightField::CalcUV(const Vector2 &p, int &xi, int &yi, float &u, float &v){
    std::pair<int, int> xy= inside(Vector3(p, 0));
    xi=xy.second;
    yi=xy.first;
    u = (p-get(yi, xi)).x;
    v = (p-get(yi, xi)).y;
}


void HeightField::Bilineaire(const Vector2 &p, double &res){
    int xi, yi;
    float u, v;
    CalcUV(p, xi, yi, u, v);
    if(xi < 0) std::cout << "bug " << p << ", " << xi << std::endl;
    res = 0;
    if(pos(yi+1, xi) >= field.size() || pos(yi, xi+1) >= field.size() || pos(yi+1, xi+1) >= field.size()) res = field[pos(yi, xi)];
    else res = (1-u)*(1-v)*field[pos(yi, xi)] + (1-u)*v*field[pos(yi+1, xi)] + u*(1-v)*field[pos(yi, xi+1)] + u*v*field[pos(yi+1, xi+1)];

}
