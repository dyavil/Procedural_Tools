#include "vegetationfield.h"

vegetationField::vegetationField(Vector2 a, Vector2 b, double defaut, double radius): ScalarField2(a, b, (b.x-a.x)/radius, (b.y-a.y)/radius, defaut)
{
    hasTree.resize(((b.x-a.x)*(b.y-a.y))/radius);
    for (unsigned int i = 0; i < hasTree.size(); ++i) hasTree[i] = false;
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


vegetationField::vegetationField(const HeightField & hf, double radius): ScalarField2(hf.a, hf.b, (hf.b.x-hf.a.x)/radius, (hf.b.y-hf.a.y)/radius, 0.0)
{
    hasTree.resize(((b.x-a.x)*(b.y-a.y))/radius);
    for (unsigned int i = 0; i < hasTree.size(); ++i) hasTree[i] = false;
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




ScalarField2 vegetationField::adaptVegetation(const HeightField & hf){
    ScalarField2 slope = hf.generateSlopeField();
    ScalarField2 wetness = hf.generateWetnessField();
    ScalarField2 illum = hf.generateIlluminationField();
    ScalarField2 streamPower = hf.generateStreamPowerField();
    ScalarField2 res = ScalarField2(a, b, w, h);
    //float xVar = (hf.b.x - hf.a.x)/hf.w;
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            //res.field[pos(i, j)] = slope(i, j);

            Vector2 coord = get(i, j);
            double currentSlope = 0.0;
            double currentWetness = 0.0;
            double currentIllum = 0.0;
            double currentStreamPower = 0.0;
            slope.Bilineaire(coord, currentSlope);
            wetness.Bilineaire(coord, currentWetness);
            illum.Bilineaire(coord, currentIllum);
            streamPower.Bilineaire(coord, currentStreamPower);
            if(hasTree[pos(i, j)]){
                 if(!(currentSlope < 3.0 && currentWetness > 2.0 && currentIllum > 10.0 && currentStreamPower < 35.0)){
                     hasTree[pos(i, j)] = false;
                     field[pos(i, j)] =  0.0;
                     //std::cout << "suppress tree" << std::endl;
                 }
            }
            if((currentSlope < 3.0 && currentWetness > 2.0 && currentIllum > 10.0 && currentStreamPower < 35.0)){
                res.field[pos(i, j)] = 25.0;
            }
        }
    }


    /*for (int i = 0; i < h; ++i) {
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
    }*/
    return res;
}


ScalarField2 vegetationField::adaptVegetation(const ScalarField2 &slope, const ScalarField2 &wetness, const ScalarField2 &illum, const ScalarField2 &streamPower){
    ScalarField2 res = ScalarField2(a, b, w, h);

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            Vector2 coord = get(i, j);
            double currentSlope = 0.0;
            double currentWetness = 0.0;
            double currentIllum = 0.0;
            double currentStreamPower = 0.0;
            slope.Bilineaire(coord, currentSlope);
            wetness.Bilineaire(coord, currentWetness);
            illum.Bilineaire(coord, currentIllum);
            streamPower.Bilineaire(coord, currentStreamPower);
            if(hasTree[pos(i, j)]){
                 if(!(currentSlope < 3.0 && currentWetness > 2.0 && currentIllum > 10.0 && currentStreamPower < 35.0)){
                     hasTree[pos(i, j)] = false;
                     field[pos(i, j)] =  0.0;
                 }
            }
            if((currentSlope < 3.0 && currentWetness > 2.0 && currentIllum > 10.0 && currentStreamPower < 35.0)){
                res.field[pos(i, j)] = 25.0;
            }
        }
    }

    return res;
}

