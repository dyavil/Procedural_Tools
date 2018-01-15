#ifndef VEGETATIONFIELD_H
#define VEGETATIONFIELD_H

#include "scalarfield2.h"
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

class vegetationField : public ScalarField2
{
public:
    vegetationField(){}
    vegetationField(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0, double radius=20.0);


    std::vector<bool> hasTree;
};

#endif // VEGETATIONFIELD_H
