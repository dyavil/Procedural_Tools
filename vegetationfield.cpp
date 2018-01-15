#include "vegetationfield.h"

vegetationField::vegetationField(Vector2 a, Vector2 b, int ww, int hh, double defaut, double radius): ScalarField2(a, b, ww, hh, defaut)
{
    hasTree.resize(ww*hh);
    for (int i = 0; i < hasTree.size(); ++i) hasTree[i] = false;
    Vec2f x_min;
    x_min[0] = a.x;
    x_min[1] = a.y;
    Vec2f x_max;
    x_max[0] = b.x;
    x_max[1] = b.y;

    uint32_t max_sample_attempts = 50;
    uint32_t seed = 1981;
    std::vector<Vec2f> samples = thinks::poissonDiskSampling(radius, x_min, x_max, max_sample_attempts, seed);
    for (unsigned int i = 0; i < samples.size(); ++i) {
        std::pair<int, int> xy= inside(Vector3(samples[i][0], samples[i][1], 0));
        field[pos(xy.first, xy.second)] = 20.0;
        hasTree[pos(xy.first, xy.second)] = true;

    }

}
