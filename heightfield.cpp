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

void HeightField::Barycentrique(const Vector2 &p, double &res) {
    int xi, yi;
    float u, v;
    CalcUV(p, xi, yi, u, v);
    if(xi < 0) std::cout << "bug " << p << ", " << xi << std::endl;
    res = 0;
    bool oppose = false;
    if(distance(p, get(yi, xi)) > distance(p, get(yi+1, xi+1))) oppose = true;
    if(pos(yi+1, xi) >= (int)field.size() || pos(yi, xi+1) >= (int)field.size() || pos(yi+1, xi+1) >= (int)field.size()) res = field[pos(yi, xi)];
    else if(!oppose) {

        float ar = area(Vector3(get(yi, xi), field[pos(yi, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        float a1 = area(Vector3(p, 0.0), Vector3(get(yi, xi), field[pos(yi, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]));
        float a2 = area(Vector3(p, 0.0), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        float a3 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi), field[pos(yi, xi)]));
        a1 = a1/ar;
        a2 = a2/ar;
        a3 = a3/ar;
        res = a1*field[pos(yi+1, xi)] + a2*field[pos(yi, xi)] + a3*field[pos(yi, xi+1)];
    }
    else{
        float ar = area(Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        float a1 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]));
        float a2 = area(Vector3(p, 0.0), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        float a3 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        a1 = a1/ar;
        a2 = a2/ar;
        a3 = a3/ar;
        res = a1*field[pos(yi+1, xi+1)] + a2*field[pos(yi+1, xi)] + a3*field[pos(yi, xi+1)];
    }

}

void HeightField::exportOBJ(const std::string & filename, bool importNormals) {

    std::vector<Vector3> vecNormales;
    std::ofstream file(filename);

    // Vertexes
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            Vector2 coord = get(i, j);
            file << "v " << coord.x << " " << coord.y << " " << field[pos(i, j)] << std::endl;
        }
    }
    file << std::endl;

    // Normales
    if(importNormals) {
        for(int i = 0; i < h; ++i) {
            for(int j = 0; j < w; ++j) {
                Vector3 norm = normal(i, j);
                if(std::find(vecNormales.begin(), vecNormales.end(), norm) == vecNormales.end()) {
                    vecNormales.push_back(norm);
                    file << "vn " << norm.x << " " << norm.y << " " << norm.z << std::endl;
                }
            }
        }
        file << std::endl;
    }

    // Faces
    for(int i = 0; i < h - 1; ++i) {
        for(int j = 0; j < w - 1; ++j) {
            int pos1 = pos(i, j) + 1;
            Vector3 n1 = normal(i, j);
            int norm1 = std::find(vecNormales.begin(), vecNormales.end(), n1) - vecNormales.begin() + 1;

            int pos2 = pos(i+1, j) + 1 ;
            Vector3 n2  = normal(i+1, j);
            int norm2 = std::find(vecNormales.begin(), vecNormales.end(), n2) - vecNormales.begin() + 1;

            int pos3 = pos(i, j+1) + 1;
            Vector3 n3 = normal(i, j+1);
            int norm3 = std::find(vecNormales.begin(), vecNormales.end(), n3) - vecNormales.begin() + 1;

            int pos4 = pos(i+1, j+1) + 1;
            Vector3 n4 = normal(i+1, j+1);
            int norm4 = std::find(vecNormales.begin(), vecNormales.end(), n4) - vecNormales.begin() + 1;

            if(importNormals) {
                file << "f " << pos1 << "//" << norm1 << " " << pos2 << "//" << norm2 << " " << pos3 << "//" << norm3 << std::endl;
                file << "f " << pos3 << "//" << norm3 << " " << pos2 << "//" << norm2 << " " << pos4 << "//" << norm4 << std::endl;
            } else {
                file << "f " << pos1 << " " << pos2 << " " << pos3 << std::endl;
                file << "f " << pos3 << " " << pos2 << " " << pos4 << std::endl;
            }
        }
    }

    file.close();
}
