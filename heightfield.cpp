#include "heightfield.h"


HeightField::HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut) : ScalarField2(a, b, ww, hh, defaut) {
    defaultHeight = defaut;
}


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


double HeightField::slope(int i, int j) {
    Vector2 tmp = gradient(i, j);
    return sqrt(tmp.x*tmp.x + tmp.y*tmp.y);
}


ScalarField2 HeightField::generateSlopeField() {
    ScalarField2 res = ScalarField2(a, b, w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.field[pos(i, j)] = slope(i, j);
        }
    }
    return res;
}


void HeightField::updateNeighborsWater(int position, ScalarField2 & waterField) const {
    Vector2 vec = coord(position);
    int i = vec.x, j = vec.y;
    double somCoeff = 0.0;

    // Calcul de la somme des coefficients
    if((i-1) >= 0 && field[pos(i-1, j)] < field[position]) { somCoeff += field[position] - field[pos(i-1, j)]; }
    if((j-1) >= 0 && field[pos(i, j-1)] < field[position]) { somCoeff += field[position] - field[pos(i, j-1)]; }
    if((i+1) < h && field[pos(i+1, j)] < field[position]) { somCoeff += field[position] - field[pos(i+1, j)]; }
    if((j+1) < w && field[pos(i, j+1)] < field[position]) { somCoeff += field[position] - field[pos(i, j+1)]; }
    if((i-1) >= 0 && (j-1) >= 0 && field[pos(i-1, j-1)] < field[position]) { somCoeff += (field[position] - field[pos(i-1, j-1)]) / M_SQRT2; }
    if((i-1) >= 0 && (j+1) < w && field[pos(i-1, j+1)] < field[position]) { somCoeff += (field[position] - field[pos(i-1, j+1)]) / M_SQRT2; }
    if((i+1) < h && (j-1) >= 0 && field[pos(i+1, j-1)] < field[position]) { somCoeff += (field[position] - field[pos(i+1, j-1)]) / M_SQRT2; }
    if((i+1) < h && (j+1) < w && field[pos(i+1, j+1)] < field[position]) { somCoeff += (field[position] - field[pos(i+1, j+1)]) / M_SQRT2; }

    // Repartition de l'eau
    if((i-1) >= 0 && field[pos(i-1, j)] < field[position]) {
        waterField.field[pos(i-1, j)] += waterField.field[position] * (field[position] - field[pos(i-1, j)]) / somCoeff;
    }
    if((j-1) >= 0 && field[pos(i, j-1)] < field[position]) {
        waterField.field[pos(i, j-1)] += waterField.field[position] * (field[position] - field[pos(i, j-1)]) / somCoeff;
    }
    if((i+1) < h && field[pos(i+1, j)] < field[position]) {
        waterField.field[pos(i+1, j)] += waterField.field[position] * (field[position] - field[pos(i+1, j)]) / somCoeff;
    }
    if((j+1) < w && field[pos(i, j+1)] < field[position]) {
        waterField.field[pos(i, j+1)] += waterField.field[position] * (field[position] - field[pos(i, j+1)]) / somCoeff;
    }
    if((i-1) >= 0 && (j-1) >= 0 && field[pos(i-1, j-1)] < field[position]) {
        waterField.field[pos(i-1, j-1)] += waterField.field[position] * ((field[position] - field[pos(i-1, j-1)]) / M_SQRT2) / somCoeff;
    }
    if((i-1) >= 0 && (j+1) < w && field[pos(i-1, j+1)] < field[position]) {
        waterField.field[pos(i-1, j+1)] += waterField.field[position] * ((field[position] - field[pos(i-1, j+1)]) / M_SQRT2) / somCoeff;
    }
    if((i+1) < h && (j-1) >= 0 && field[pos(i+1, j-1)] < field[position]) {
        waterField.field[pos(i+1, j-1)] += waterField.field[position] * ((field[position] - field[pos(i+1, j-1)]) / M_SQRT2) / somCoeff;
    }
    if((i+1) < h && (j+1) < w && field[pos(i+1, j+1)] < field[position]) {
        waterField.field[pos(i+1, j+1)] += waterField.field[position] * ((field[position] - field[pos(i+1, j+1)]) / M_SQRT2) / somCoeff;
    }
}


ScalarField2 HeightField::generateDrainageArea(float initialAmount) const {
    ScalarField2 res = ScalarField2(a, b, w, h);
    std::vector<std::array<double, 2>> vecHeights(field.size());

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            int position = pos(i,j);
            res.field[position] = initialAmount;
            vecHeights[position] = { field[position], (double)position };
        }
    }

    std::sort(vecHeights.rbegin(), vecHeights.rend());

    for(unsigned int i = 0; i < vecHeights.size(); ++i) {
        updateNeighborsWater(vecHeights[i][1], res);
    }

    return res;
}


ScalarField2 HeightField::generateWetnessField() {
    ScalarField2 stp = generateSlopeField();
    ScalarField2 dr = generateDrainageArea();
    ScalarField2 res = ScalarField2(a, b, w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.field[pos(i, j)] = log(dr.field[pos(i, j)]/(1.0+stp.field[pos(i, j)]));
        }
    }
    return res;
}


ScalarField2 HeightField::generateStreamPowerField() {
    ScalarField2 dra = generateDrainageArea();
    ScalarField2 slp = generateSlopeField();
    ScalarField2 res = ScalarField2(a, b, w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.field[pos(i, j)] = sqrt(dra.field[pos(i, j)])*slp.field[pos(i, j)];
        }
    }
    return res;
}


ScalarField2 HeightField::generateIlluminationField(int nbPoints) {
    ScalarField2 res = ScalarField2(a, b, w, h);

    Vector2 centre = getCenter();
    VAR_TYPE radius = distance(centre, a) + distance(centre, a)/10;
    Sphere sphere(Vector3(centre, 0.0) , radius);
    Vector3 normaleSphere(centre, defaultHeight);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);

    std::vector<Vector3> listPoints(nbPoints);

    for(int i = 0; i < nbPoints; ++i) {
        double theta = 2 * M_PI * uniform(generator);
        double phi = acos(1 - 2 * uniform(generator));

        double x = sphere.center.x + sphere.radius * sin(phi) * cos(theta);
        double y = sphere.center.y + sphere.radius * sin(phi) * sin(theta);
        double z = sphere.center.z + sphere.radius * cos(phi);

        Vector3 point(x, y, z);

        // Si point dans l'hemisphere inférieur, on l'inverse
        if(point.z < defaultHeight) {
            point.x = -point.x;
            point.y = -point.y;
            point.z = -point.z;
        }

        listPoints[i] = point;
    }

    /*
    std::cout << "center : " << sphere.center << std::endl;
    std::cout << "radius : " << sphere.radius << std::endl;
    for(unsigned int i = 0; i < listPoints.size(); ++i) {
        std::cout << "p : " << listPoints[i] << std::endl;
    }
    */

    // NOT FINISHED
    // TODO : Ray Tracing

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
