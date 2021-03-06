#include "scalarfield2.h"
#include "include/FastNoise.h"

ScalarField2::ScalarField2(Vector2 a, Vector2 b, int ww, int hh, double defaut) : Array2(a, b, ww, hh)
{
    field.resize(ww*hh);
    for (int i = 0; i < ww*hh; ++i) {
        field[i] = defaut;
    }
}


bool ScalarField2::load(QString path, double zmin, double zmax){
    QImage img = QImage(path);
    h=img.height();
    w=img.width();
    field.resize(img.width()*img.height());
    if(img.isNull()) return false;
    for (int i = 0; i < img.height(); ++i) {
        for (int j = 0; j < img.width(); ++j) {
            QColor clrC( img.pixel( j, i ) );
            int ii = img.height()-i-1;
            field[pos(ii, j)] = zmax-(((zmax-zmin)*clrC.black())/(255));
        }
    }
    return true;
}


ScalarField2 ScalarField2::sfNormalize() const {
    ScalarField2 res = ScalarField2(a, b, w, h);
    double max = *std::max_element(field.begin(), field.end());
    double min = *std::min_element(field.begin(), field.end());

    for (int i = 0; i < h*w; ++i) {
        res.field[i] = field[i] - min ;
    }
    for (int i = 0; i < h*w; ++i) {
        res.field[i] /= (max-min) ;
    }

    return res;
}


QImage ScalarField2::render(){
    std::vector<double>::iterator result;
    result = std::max_element(field.begin(), field.end());
    double zm = *result;
    result = std::min_element(field.begin(), field.end());
    double zmin = *result;

    QImage res = QImage(w, h, QImage::Format_RGB32);
    QRgb val;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int ii = h-i-1;
            double vt = ((field[pos(i, j)]-zmin)/(zm-zmin));
            int v = floor(vt * 255.0);
            val = qRgb(v, v, v);
            res.setPixel(j, ii, val);
        }
    }
    return res;
}


Vector2 ScalarField2::gradient(int i, int j) const{
    double aa = 2.0*((b.x-a.x)/(double)w);
    double dx;
    if(j+1 >= w) dx = (field[pos(i, j)] - field[pos(i, j-1)])/(aa/2.0);
    else if(j-1 < 0) dx = (field[pos(i, j+1)] - field[pos(i, j)])/(aa/2.0);
    else dx = (field[pos(i, j+1)] - field[pos(i, j-1)])/aa;
    aa = 2.0*((b.y-a.y)/h);
    double dy;
    if(i+1 >= h) dy = (field[pos(i, j)] - field[pos(i-1, j)])/(aa/2.0);
    else if(i-1 < 0) dy = (field[pos(i+1, j)] - field[pos(i, j)])/(aa/2.0);
    else dy = (field[pos(i+1, j)] - field[pos(i-1, j)])/aa;
    return Vector2(dx, dy);
}


void ScalarField2::noiseMap(int pas, float rapport, int seed){
    FastNoise myNoise;
    w = (b.x - a.x)/16;
    h = (b.y - a.y)/16;
    field.resize(w*h);
    myNoise.SetNoiseType(FastNoise::Perlin);
    myNoise.SetSeed(seed);
    double tmpp = sqrt((b-a).length())*7100.0;
    double freq = (((b-a).length())/tmpp)/rapport;

    double heightFact = sqrt((b-a).length())*(1000.0/(float)h)*rapport;

    myNoise.SetFrequency(freq);
    for (int i = 0; i < h*w; ++i) {
        field[i]= 0.0;
    }

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            field[pos(i, j)] = (myNoise.GetNoise(j,i));
        }
    }
    double coef = 0.5;
    double coefM = 1.0;
    for (int oc = 0; oc < pas; ++oc) {
        freq = freq *1.4;
        myNoise.SetFrequency(freq);
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                field[pos(i, j)] += (coef*myNoise.GetNoise(coefM*j,coefM*i));
            }
        }
        coef /=2.0;
        coefM *= 2.0;
    }

    std::vector<double>::iterator result;
    result = std::max_element(field.begin(), field.end());
    double zm = *result;
    result = std::min_element(field.begin(), field.end());
    double zmin = *result;
    std::cout << "min : " << zmin << ", " << zm <<std::endl;
    for (int i = 0; i < h*w; ++i) {
        field[i]= field[i] -= zmin;
        field[i] *= heightFact;
    }
    result = std::max_element(field.begin(), field.end());
    zm = *result;
    result = std::min_element(field.begin(), field.end());
    zmin = *result;
    std::cout << "min : " << zmin << ", " << zm <<std::endl;

}


void ScalarField2::CalcUV(const Vector2 &p, int &xi, int &yi, double &u, double &v) const{
    std::pair<int, int> xy= inside(Vector3(p, 0));
    xi=xy.second;
    yi=xy.first;
    u = (p-get(yi, xi)).x/((b.x-a.x)/(w-1));
    v = (p-get(yi, xi)).y/((b.y-a.y)/(h-1));
}


void ScalarField2::Bilineaire(const Vector2 &p, double &res) const{
    int xi, yi;
    double u, v;
    CalcUV(p, xi, yi, u, v);

    if(xi < 0) std::cout << "bug " << p << ", " << xi << std::endl;
    res = (1.0-u)*(1.0-v)*field[pos(yi, xi)] + (1.0-u)*v*field[pos(yi+1, xi)] + u*(1.0-v)*field[pos(yi, xi+1)] + u*v*field[pos(yi+1, xi+1)];

}

void ScalarField2::Barycentrique(const Vector2 &p, double &res) {
    int xi, yi;
    double u, v;
    CalcUV(p, xi, yi, u, v);

    if(xi < 0) std::cout << "bug " << p << ", " << xi << std::endl;
    res = 0;
    bool oppose = false;
    static int count = 0;
    if(distance(p, get(yi, xi)) >= distance(p, get(yi+1, xi+1))) {
        oppose = true;
        count++;
    }

    if(pos(yi+1, xi) >= (int)field.size() || pos(yi, xi+1) >= (int)field.size() || pos(yi+1, xi+1) >= (int)field.size()) res = field[pos(yi, xi)];
    else if(!oppose) {

        double ar = area(Vector3(get(yi, xi), field[pos(yi, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        double a1 = area(Vector3(p, 0.0), Vector3(get(yi, xi), field[pos(yi, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]));
        double a2 = area(Vector3(p, 0.0), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        double a3 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi), field[pos(yi, xi)]));
        a1 = a1/ar;
        a2 = a2/ar;
        a3 = a3/ar;
        res = a1*field[pos(yi+1, xi)] + a2*field[pos(yi, xi)] + a3*field[pos(yi, xi+1)];

    } else {
        double ar = area(Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        double a1 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]));
        double a2 = area(Vector3(p, 0.0), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        double a3 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        a1 = a1/ar;
        a2 = a2/ar;
        a3 = a3/ar;
        res = a1*field[pos(yi+1, xi+1)] + a2*field[pos(yi+1, xi)] + a3*field[pos(yi, xi+1)];

    }

}

