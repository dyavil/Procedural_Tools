#ifndef ARRAY2_H
#define ARRAY2_H
#include "geometry.h"
#include <utility>

class Array2 : public Box2
{
public:
    Array2() {}
    Array2(Vector2 a, Vector2 b, int ww, int hh);

    /**
     * @brief pos Retourne la position correspondante à (i,j) dans un tableau 1D
     * @param i ligne
     * @param j colonne
     * @return position dans un tableau 1D
     */
    int pos(int i, int j) const { return (i*w + j); }

    /**
     * @brief coord Retourne la position correspondante à pos dans un tableau 2D
     * @param pos position 1D recherchée
     * @return position dans un tableau 2D
     */
    Vector2 coord(int pos) const { return Vector2(pos/h , pos%w); }

    /**
     * @brief get Retourne les coordonnées (i,j) mises à l'échelle du tableau
     * @param i ligne
     * @param j colonne
     * @return coordonnées (i,j) réelles
     */
    Vector2 get(int i, int j) const;

    /**
     * @brief getCenter Retourne le centre du tableau
     * @return centre (en coordonnées réelles) du tableau
     */
    Vector2 getCenter() const;

    /**
     * @brief inside Retourne les identifiant i, j du point bas gauche
     *        le plus proche de v3
     * @param v3
     * @return
     */
    std::pair<int, int> inside(const Vector3 & v3) const;

    int h;
    int w;
};

#endif // ARRAY2_H
