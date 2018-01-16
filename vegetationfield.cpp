#include "vegetationfield.h"

vegetationField::vegetationField(Vector2 a, Vector2 b, double defaut, double radius): ScalarField2(a, b, (b.x-a.x), (b.y-a.y), defaut)
{
    hasTree.resize((b.x-a.x)*(b.y-a.y));
    for (int i = 0; i < hasTree.size(); ++i) hasTree[i] = false;
    Vec2f x_min;
    x_min[0] = a.x;
    x_min[1] = a.y;
    Vec2f x_max;
    x_max[0] = b.x;
    x_max[1] = b.y;
    treeWidth = radius;
    uint32_t max_sample_attempts = 50;
    uint32_t seed = 1994;
    std::vector<Vec2f> samples = thinks::poissonDiskSampling(radius, x_min, x_max, max_sample_attempts, seed);
    for (unsigned int i = 0; i < samples.size(); ++i) {
        std::pair<int, int> xy= inside(Vector3(samples[i][0], samples[i][1], 0));
        field[pos(xy.first, xy.second)] = 20.0;
        hasTree[pos(xy.first, xy.second)] = true;

    }

}


void vegetationField::adaptVegetation(HeightField hf){
    ScalarField2 slope = hf.generateSlopeField();
    ScalarField2 wetness = hf.generateWetnessField();
    ScalarField2 illum = hf.generateIlluminationField();
    //float xVar = (hf.b.x - hf.a.x)/hf.w;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            //res.field[pos(i, j)] = slope(i, j);
            if(hasTree[pos(i, j)]){
                 Vector2 coord = get(i, j);
                 double currentSlope = 0.0;
                 double currentWetness = 0.0;
                 double currentIllum = 0.0;
                 slope.Bilineaire(coord, currentSlope);
                 wetness.Bilineaire(coord, currentWetness);
                 illum.Bilineaire(coord, currentIllum);
                 if(!(currentSlope < 3.2 && currentWetness > 1.5 && currentIllum > 5.0)){
                     hasTree[pos(i, j)] = false;
                     field[pos(i, j)] =  0.0;
                     //std::cout << "suppress tree" << std::endl;
                 }
            }
        }
    }


    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            //res.field[pos(i, j)] = slope(i, j);
            if(hasTree[pos(i, j)]){
                double cSize = field[pos(i, j)];
                for (int k = 0; k < treeWidth/2 - 1; ++k) {
                    cSize /= 2.0;
                    if(i-k > 0 && j-k > 0) field[pos(i-k, j-k)] =cSize;
                    if(i-k > 0 && j+k < w) field[pos(i-k, j+k)] =cSize;
                    if(i+k < h && j-k > 0) field[pos(i+k, j-k)] =cSize;
                    if(i+k < h && j+k < w) field[pos(i+k, j+k)] =cSize;
                }
            }
        }
    }


}


