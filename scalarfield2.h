#ifndef SCALARFIELD_H
#define SCALARFIELD_H

#include "array2.h"
#include <vector>
#include <QImage>
#include <time.h>
#include <stdlib.h>

class ScalarField2 : public Array2
{
public:
    ScalarField2() {}
    ScalarField2(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);

    /**
     * @brief load chargement depuis un fichier image
     * @param path de l'image
     * @param zmin hauteur minimum du terrain
     * @param zmax hauteur maximum du terrain
     * @return false si echec
     */
    bool load(QString path, double zmin, double zmax);
    void noiseMap(int pas, float rapport = 1.0, int seed = 1337);

    /**
     * @brief sfNormalize Normalise le scalarField entre 0 et 1
     * @return scalarField normalisé
     */
    ScalarField2 sfNormalize() const;

    /**
     * @brief render tranforme le scalarfield en image
     * @return l'image du scalarfield
     */
    QImage render();

    /**
     * @brief gradient calcul du gradient en i, j
     * @param i
     * @param j
     * @return la valeur du gradient
     */
    Vector2 gradient(int i, int j) const;

    /**
     * @brief setVal allocation de valeur
     * @param i
     * @param j
     * @param val valeur alouée
     */
    void setVal(int i, int j, double val) {field[pos(i, j)] = val;}

    /**
     * @brief CalcUV calcul les parametre de l'interpolation bilinéaire
     * @param p point cible
     * @param xi j sur la carte
     * @param yi i sur la carte
     * @param u
     * @param v
     */
    void CalcUV(const Vector2 & p , int & xi, int & yi, double & u, double & v) const;

    /**
     * @brief Bilineaire interpolation bilinéaire
     * @param p point cible
     * @param res hauteur interpolée
     */
    void Bilineaire(const Vector2 & p, double & res) const;

    /**
     * @brief Barycentrique interpolation barycentrique
     * @param p point cible
     * @param res hauteur interpolée
     */
    void Barycentrique(const Vector2 & p, double & res);

    /**
     * @brief field champ de valeurs
     */
    std::vector<double> field;
};

#endif // SCALARFIELD_H
