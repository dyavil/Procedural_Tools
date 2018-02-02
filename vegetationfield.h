#ifndef VEGETATIONFIELD_H
#define VEGETATIONFIELD_H

#include "heightfield.h"
#include "time.h"
#include "include/thinks/poissonDiskSampling.hpp"

#define divideFactor 4.0

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

/**
 * @brief The Tree class
 * Classe représentant les arbres
 * deux espèces par defaut
 */
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

static std::vector<Tree> trees = {Tree(2.0, 12.0, 2.5, 12.0, 15.0, "lowpolytree4.obj"), Tree(1.8, 8.0, 4.0, 25.0, 10.0,"lowpolytree3.obj")};

/**
 * @brief The vegetationField class
 * classe représentant la génération de la
 * végétation par distribution de poisson
 */
class vegetationField : public ScalarField2
{
public:
    vegetationField(){}
    vegetationField(const HeightField & hf, const ScalarField2 & slope, const ScalarField2 & wetness, const ScalarField2 & illum, const ScalarField2 & streamPower);

    /**
     * @brief genImage genere le scalarfield de disposition des arbres
     * @return le scalarfield généré
     */
    ScalarField2 genImage();

    std::vector<bool> hasTree;
private:
    /**
     * @brief checkNeighbor teste la compétition par rapport aux arbres voisins
     * @param i position de i l'arbre
     * @param j position de j l'arbre
     * @param treeId id du type d'arbre
     * @return vrai si competition
     */
    bool checkNeighbor(int i, int j, int treeId);
};

#endif // VEGETATIONFIELD_H
