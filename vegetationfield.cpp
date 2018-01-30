#include "vegetationfield.h"




vegetationField::vegetationField(const HeightField & hf, const ScalarField2 & slope, const ScalarField2 & wetness, const ScalarField2 & illum, const ScalarField2 & streamPower): ScalarField2(hf.a, hf.b, (hf.b.x-hf.a.x)/(divideFactor), (hf.b.y-hf.a.y)/(divideFactor), 0.0)
{
    hasTree.resize(((b.x-a.x)*(b.y-a.y))/(divideFactor));
    field.resize(((b.x-a.x)*(b.y-a.y))/(divideFactor));
    for (unsigned int i = 0; i < hasTree.size(); ++i) hasTree[i] = false;
    Vec2f x_min;
    x_min[0] = a.x;
    x_min[1] = a.y;
    Vec2f x_max;
    x_max[0] = b.x;
    x_max[1] = b.y;

    uint32_t max_sample_attempts = 50;
    uint32_t seed = 1994;

    srand(time(NULL));
    for (unsigned int tr = 0; tr < trees.size(); ++tr) {
        VAR_TYPE radius = trees[tr].widthT;
        std::vector<Vec2f> samples = thinks::poissonDiskSampling(radius, x_min, x_max, max_sample_attempts, seed);
        for (unsigned int i = 0; i < samples.size(); ++i) {
            double rnd = ((double) rand() / (RAND_MAX));
            std::pair<int, int> xy= inside(Vector3(samples[i][0], samples[i][1], 0));
            Vector2 coord = Vector2(samples[i][0], samples[i][1]);
            double currentSlope = 0.0;
            double currentWetness = 0.0;
            double currentIllum = 0.0;
            double currentStreamPower = 0.0;
            slope.Bilineaire(coord, currentSlope);
            wetness.Bilineaire(coord, currentWetness);
            illum.Bilineaire(coord, currentIllum);
            streamPower.Bilineaire(coord, currentStreamPower);
            bool valid = false;
            if((currentSlope < trees[tr].slopeMax && currentWetness > trees[tr].wetnessMin && currentIllum > trees[tr].lightMin && currentStreamPower < trees[tr].streamPowerMax) && !checkNeighbor(xy.first, xy.second, tr)) valid = true;
            if(hasTree[pos(xy.first, xy.second)] && rnd > 0.5 ){
                if(valid) field[pos(xy.first, xy.second)] = tr;
            }
            else{
                if(valid)
                {
                    field[pos(xy.first, xy.second)] = tr;
                    hasTree[pos(xy.first, xy.second)] = true;
                }
            }

        }
    }
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
                 if(!(currentSlope < trees[(int)field[pos(i, j)]].slopeMax && currentWetness > trees[(int)field[pos(i, j)]].wetnessMin && currentIllum > trees[(int)field[pos(i, j)]].lightMin && currentStreamPower < trees[(int)field[pos(i, j)]].streamPowerMax)){
                     hasTree[pos(i, j)] = false;
                     field[pos(i, j)] =  0.0;
                     //std::cout << "suppress tree" << std::endl;
                 }
            }

            if((currentSlope < trees[(int)field[pos(i, j)]].slopeMax && currentWetness > trees[(int)field[pos(i, j)]].wetnessMin && currentIllum > trees[(int)field[pos(i, j)]].lightMin && currentStreamPower < trees[(int)field[pos(i, j)]].streamPowerMax)){
                res.field[pos(i, j)] = 25.0;
            }
        }
    }

    return res;
}


ScalarField2 vegetationField::genImage(){
    ScalarField2 res = ScalarField2(a, b, w, h);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            if(hasTree[pos(i, j)]){
                res.field[pos(i, j)] = 25.0;
            }
            else res.field[pos(i, j)] = 0.0;
        }
    }
    return res;
}


bool vegetationField::checkNeighbor(int i, int j, int treeId){
    int around = ceil(trees[treeId].widthT/divideFactor/2);
    for (int l = -around; l <= around; ++l) {
        for (int k = -around; k <= around; ++k) {
            if(((i+1) >= 0) && ((j+k) >= 0) && ((i+1) < h) && ((j+k) < w)){
                  if(hasTree[pos(i+l, j+k)]) return true;
                  //std::cout << pos(i+l, j+k) << std::endl;
            }
        }
    }
    return false;
}
