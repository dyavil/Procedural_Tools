#include "heightfield.h"

HeightField::HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut) : ScalarField2(a, b, ww, hh, defaut) {}


Vector3 HeightField::normalOld(int i, int j) {
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


Vector3 HeightField::normal(int i, int j) {

    Vector3 v1, v3, v4, v5, v7, v8;
    Vector3 origin = Vector3(get(i, j), field[pos(i, j)]);
    if((i+1) < h) { v1 = Vector3(get(i+1, j), field[pos(i+1, j)]) - origin; }
    if((j+1) < w) { v3 = Vector3(get(i, j+1), field[pos(i, j+1)]) - origin; }
    if((i-1) >= 0 && (j+1) < w) { v4 = Vector3(get(i-1, j+1), field[pos(i-1, j+1)]) - origin; }
    if((i-1) >= 0) { v5 = Vector3(get(i-1, j), field[pos(i-1, j)]) - origin; }
    if((j-1) >= 0) { v7 = Vector3(get(i, j-1), field[pos(i, j-1)]) - origin; }
    if((i+1) < h && (j-1) >=0) { v8 = Vector3(get(i+1, j-1), field[pos(i+1, j-1)]) - origin; }

    Vector3 n1, n2, n3, n4, n5, n6;
    int somN = 0;
    if((i+1) < h && (j+1) < w) {
        n1 = cross(v3, v1);
        somN++;
    }
    if((i-1) >= 0 && (j+1) < w) {
        n2 = cross(v4, v3);
        n3 = cross(v5, v3);
        somN += 2;
    }
    if((i-1) >= 0 && (j-1) >= 0) {
        n4 = cross(v7, v5);
        somN++;
    }
    if((i+1) < h && (j-1) >= 0) {
        n5 = cross(v8, v7);
        n6 = cross(v1, v8);
        somN += 2;
    }

    return normalize((n1+n2+n3+n4+n5+n6)/somN);
}


void HeightField::CalcUV(const Vector2 &p, int &xi, int &yi, float &u, float &v) {
    std::pair<int, int> xy= inside(Vector3(p, 0));
    xi=xy.second;
    yi=xy.first;
    u = (p-get(yi, xi)).x;
    v = (p-get(yi, xi)).y;
}


void HeightField::Bilineaire(const Vector2 &p, double &res) {
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


double HeightField::slope(int i, int j) {
    Vector2 tmp = gradient(i, j);
    return sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
}


ScalarField2 HeightField::generateSlopeField() {
    ScalarField2 res = ScalarField2(a, b, w, h);
    int t, tt;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.field[pos(i, j)] = slope(i, j)/20;
            t = j;
        }
        tt = i;
    }
    std::cout << pos(t, tt)  << ", " << field.size() << std::endl;
    return res;
}


void HeightField::exportOBJ(const std::string & filename, bool importNormals) {

    std::unordered_map<Vector3, int, Vector3Hasher> mapNormales;
    std::ofstream file(filename);

    // Vertexes
    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            Vector2 coord = get(i, j);
            file << "v " << coord.x << " " << field[pos(i, j)] << " " << coord.y << std::endl;
        }
    }
    file << std::endl;

    // Normales
    if(importNormals) {
        for(int i = 0; i < h; ++i) {
            for(int j = 0; j < w; ++j) {
                Vector3 norm = normal(i, j);

                if(mapNormales.find(norm) == mapNormales.end()) {
                    file << "vn " << norm.x << " " << norm.z << " " << norm.y << std::endl;
                    mapNormales.insert(std::make_pair(norm, mapNormales.size()+1));
                }
            }
        }
        file << std::endl;
    }

    // Faces
    for(int i = 0; i < h - 1; ++i) {
        for(int j = 0; j < w - 1; ++j) {
            int pos1 = pos(i, j) + 1;
            int pos2 = pos(i, j+1) + 1 ;
            int pos3 = pos(i+1, j) + 1;
            int pos4 = pos(i+1, j+1) + 1;

            if(importNormals) {
                int norm1 = mapNormales.find(normal(i, j))->second;
                int norm2 = mapNormales.find(normal(i, j+1))->second;
                int norm3 = mapNormales.find(normal(i+1, j))->second;
                int norm4 = mapNormales.find(normal(i+1, j+1))->second;

                file << "f " << pos1 << "//" << norm1 << " " << pos2 << "//" << norm2 << " " << pos3 << "//" << norm3 << std::endl;
                file << "f " << pos2 << "//" << norm2 << " " << pos4 << "//" << norm4 << " " << pos3 << "//" << norm3 << std::endl;
            } else {
                file << "f " << pos1 << " " << pos2 << " " << pos3 << std::endl;
                file << "f " << pos2 << " " << pos4 << " " << pos3 << std::endl;
            }
        }
    }

    file.close();
}
