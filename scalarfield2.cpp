#include "scalarfield2.h"

ScalarField2::ScalarField2(Vector2 a, Vector2 b, int ww, int hh, double defaut) : Array2(a, b, ww, hh)
{
    field.resize(ww*hh);
    for (int i = 0; i < ww*hh; ++i) {
             field[i] = defaut;
    }

}


bool ScalarField2::load(QImage & im, Vector2 a, Vector2 b, double za, double zb){
    this->a=a;
    this->b=b;
    h=im.height();
    w=im.width();
    field.resize(im.width()*im.height());
    if(im.isNull()) return false;
    for (int i = 0; i < im.height(); ++i) {
        for (int j = 0; j < im.width(); ++j) {
            QColor clrC( im.pixel( j, i ) );
            int ii = im.height()-i-1;
            field[pos(ii, j)] = zb-(((zb-za)*clrC.black())/(255));
        }
    }

    return true;
}


QImage ScalarField2::render(){
    std::vector<double>::iterator result;
    result = std::max_element(field.begin(), field.end());
    double zm = *result;

    QImage res = QImage(w, h, QImage::Format_RGB32);
    QRgb val;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            int ii = h-i-1;
            double vt = ((field[pos(i, j)])/(zm));
            int v = floor(vt * 255.0);
            val = qRgb(v, v, v);
            res.setPixel(j, ii, val);
        }
    }
    return res;
}


Vector2 ScalarField2::gradient(int i, int j){
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

double ScalarField2::interpolation_cos1D(double a, double b, double x) {

   double k = (1 - cos(x * M_PI)) / 2;

    return a * (1 - k) + b * k;
}

double ScalarField2::interpolation_cos2D(double a, double b, double c, double d, double x, double y) {
   double y1 = interpolation_cos1D(a, b, x);
   double y2 = interpolation_cos1D(c, d, x);
   return  interpolation_cos1D(y1, y2, y);
}

double ScalarField2::fonction_bruit2D(double x, double y, int pas2D) {
   int i = (int) (y / pas2D);
   int j = (int) (x / pas2D);
   return interpolation_cos2D(field[pos(i, j)], field[pos(i + 1, j)], field[pos(i, j + 1)], field[pos(i + 1, j + 1)], fmod(y / pas2D, 1), fmod(x / pas2D, 1));
}

void ScalarField2::noiseMap(int pas){

    srand(time(NULL));
    for (int i = 0; i < h*w; ++i) {
        field[i]= 0.1;
    }
    for (int q = 0; q < 1; ++q) {

        std::vector<double> noise;
        int lg = (int) ceil((w+1)/ pas);
        int ht = (int) ceil((h+1)/ pas);
        noise.resize(lg*ht);
        for (unsigned int i = 0; i < lg*ht; ++i) {
            noise[i] = ((double) rand() / (RAND_MAX))*0.8;
        }
        //std::cout << "noi " << noise[0] << std::endl;

        ScalarField2 nn = ScalarField2(a, b, lg, ht);
        nn.field = noise;
        double pasX = (b.x - a.x)/(float)w;
        double pasY = (b.y - a.y)/(float)h;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                Vector2 t = a+Vector2(i*pasX, j*pasY);

                double re = 0.0;
                nn.Barycentrique(t, re);
                re = nn.fonction_bruit2D(inside(Vector3(t, 0.0)).first, inside(Vector3(t, 0.0)).second, pas);
                //field[pos(i, j)] += re;
                field[pos(i, j)] = std::max(re, field[pos(i, j)]);
            }
        }


        for(int t = 0; t < 0; ++t){

            /*nn.w = nn.w*2;
            nn.h = nn.h*2;
            noise.resize(nn.h*nn.w);
            for (int i = 0; i < nn.field.size(); ++i) {
                noise[i]= nn.field[i];
            }
            for (int i = nn.field.size(); i < nn.field.size()*2; ++i) {
                noise[i]= nn.field[i-nn.field.size()];
            }
            for (int i = nn.field.size()*2; i < nn.field.size()*3; ++i) {
                noise[i]= nn.field[i-nn.field.size()*2];
            }
            for (int i = nn.field.size()*3; i < nn.field.size()*4; ++i) {
                noise[i]= nn.field[i-nn.field.size()*3];
            }*/
            /*double last = noise[noise.size()-1];
            for (int i = 1; i < noise.size(); ++i) {
                noise[i] = noise[i-1];
            }
            noise[0] = last;*/
            //std::cout << nn.field.size()*2 << noise.size() << nn.h*nn.w << std::endl;
            /*lg = (int) ceil(w/ (pas*1.0));
            ht = (int) ceil(h/ (pas*1.0));
            noise.resize(lg*ht);
            srand(time(NULL));
            for (unsigned int i = 0; i < lg*ht; ++i) {
                noise[i] = ((double) rand() / (RAND_MAX))*0.5;
            }*/
            //nn.field = noise;
            for (int i = 0; i < h; i+=2) {
                for (int j = 0; j < w; j+=2) {
                    Vector2 t = a+Vector2(i*pasX, j*pasY);
                    double re = 0.0;
                    nn.Barycentrique(t, re);
                    re = nn.fonction_bruit2D(inside(Vector3(t, 0.0)).first, inside(Vector3(t, 0.0)).second, 1);
                    //field[pos(i, j)] += re;
                    field[pos(i, j)] = std::max(re, field[pos(i, j)]);
                }
            }
        }
    }

    for (int i = 0; i < field.size(); ++i) {
       // field[i] = field[i]/10;
    }
}


void ScalarField2::CalcUV(const Vector2 &p, int &xi, int &yi, double &u, double &v){
    std::pair<int, int> xy= inside(Vector3(p, 0));
    xi=xy.second;
    yi=xy.first;
    u = (p-get(yi, xi)).x;
    v = (p-get(yi, xi)).y;
}


void ScalarField2::Bilineaire(const Vector2 &p, double &res){
    int xi, yi;
    double u, v;
    CalcUV(p, xi, yi, u, v);
    if(xi < 0) std::cout << "bug " << p << ", " << xi << std::endl;
    res = 0;
    if(pos(yi+1, xi) >= (int)field.size() || pos(yi, xi+1) >= (int)field.size() || pos(yi+1, xi+1) >= (int)field.size()) res = field[pos(yi, xi)];
    else res = (1-u)*(1-v)*field[pos(yi, xi)] + (1-u)*v*field[pos(yi+1, xi)] + u*(1-v)*field[pos(yi, xi+1)] + u*v*field[pos(yi+1, xi+1)];

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
    //std::cout << p << ", " << distance(p, get(yi, xi)) << ", " << get(yi, xi) << ", " << distance(p, get(yi+1, xi+1)) << std::endl;
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
        /*if(a1 < 0) std::cout << "1 1 " << a1 << std::endl;
        if(a2 < 0) std::cout << "1 2 " << a2 << std::endl;
        if(a3 < 0) std::cout << "1 3 " << a3 << std::endl;*/
    }
    else{
        double ar = area(Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        double a1 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]));
        double a2 = area(Vector3(p, 0.0), Vector3(get(yi, xi+1), field[pos(yi, xi+1)]), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]));
        double a3 = area(Vector3(p, 0.0), Vector3(get(yi+1, xi+1), field[pos(yi+1, xi+1)]), Vector3(get(yi+1, xi), field[pos(yi+1, xi)]));
        a1 = a1/ar;
        a2 = a2/ar;
        a3 = a3/ar;
        res = a1*field[pos(yi+1, xi+1)] + a2*field[pos(yi+1, xi)] + a3*field[pos(yi, xi+1)];

        /*if(a1 < 0) std::cout << "2 1 " << a1 << std::endl;
        if(a2 < 0) std::cout << "2 2 " << a2 << std::endl;
        if(a3 < 0) std::cout << "2 3 " << a3 << std::endl;*/
    }
    //std::cout << count << std::endl;
}

