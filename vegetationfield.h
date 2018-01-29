#ifndef VEGETATIONFIELD_H
#define VEGETATIONFIELD_H

#include "heightfield.h"
#include "time.h"
#include "include/thinks/poissonDiskSampling.hpp"

template <typename T, std::size_t N>
class Vec
{
public:
  typedef T value_type;
  static const std::size_t size = N;
  Vec() {}
  T& operator[](std::size_t i) { return _data[i]; }
  const T& operator[](std::size_t i) const { return _data[i]; }
private:
  T _data[N];
};

typedef Vec<double, 2> Vec2f;


class Tree{
public:
    Tree(VAR_TYPE wM, VAR_TYPE lM, VAR_TYPE sM, VAR_TYPE spM, VAR_TYPE wT, QString obp):wetnessMin(wM), lightMin(lM), slopeMax(sM), streamPowerMax(spM), widthT(wT), objPath(obp){}
    ~Tree(){}

    VAR_TYPE wetnessMin;
    VAR_TYPE lightMin;
    VAR_TYPE slopeMax;
    VAR_TYPE streamPowerMax;
    VAR_TYPE widthT;
    QString objPath;
};

static std::vector<Tree> trees = {Tree(3.0, 12.0, 2.5, 35.0, 15.0, "lowpolytree4.obj"), Tree(2.0, 8.0, 4.0, 40.0, 10.0,"lowpolytree3.obj")};


class vegetationField : public ScalarField2
{
public:
    vegetationField(){}
    vegetationField(Vector2 a, Vector2 b, double defaut=0.0);
    vegetationField(const HeightField & hf);

    ScalarField2 adaptVegetation(const HeightField & hf);
    ScalarField2 adaptVegetation(const ScalarField2 & slope, const ScalarField2 & wetness, const ScalarField2 & illum, const ScalarField2 & streamPower);

    std::vector<bool> hasTree;
};

#endif // VEGETATIONFIELD_H
