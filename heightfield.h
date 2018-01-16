#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <chrono>
#include <fstream>
#include <unordered_map>
#include "scalarfield2.h"
#include "vector"
#include <random>

class HeightField : public ScalarField2
{
public:
    HeightField() {}
    HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0);

    bool load(QImage & im, Vector2 a, Vector2 b, double za, double zb);

    // Ancien calcul de normale (plus utilisé)
    Vector3 normalOld(int i, int j);

    /**
     * @brief normal Calcule la normale en un point
     * @param i ligne
     * @param j colonne
     * @return la normale a ce point
     */
    Vector3 normal(int i, int j);

    /**
     * @brief underTerrain Indique si un point se situe en dessus ou en dessous du terrain
     * @param vec position (réelles) du point que l'on teste
     * @return true si le point(x, y, z est situé en dessous du terrain)
     */
    bool underTerrain(Vector3 & vec);

    /**
     * @brief slope
     * @param i
     * @param j
     * @return
     */
    double slope(int i, int j);

    /**
     * @brief generateSlopeField
     * @return
     */
    ScalarField2 generateSlopeField();

    /**
     * @brief updateNeighborsWater Met à jour la quantité d'eau de la drainageArea en un point
     * @param position position à mettre à jour
     * @param waterField drainageArea mise à jour
     */
    void updateNeighborsWater(int position, ScalarField2 & waterField) const;

    /**
     * @brief generateDrainageArea Génère la drainageArea du heightfield
     * @param initialAmount montant initial d'eau (par défaut 1.0)
     * @return la drainageArea du heightfield
     */
    ScalarField2 generateDrainageArea(float initialAmount = 1.0) const;

    /**
     * @brief generateWetnessField Génère le wetnessField du heightfield
     * @return le wetnessField du heightfield
     */
    ScalarField2 generateWetnessField();

    /**
     * @brief generateStreamPowerField Génère le streamPowerField du heightfield
     * @return le streamPowerField du heightfield
     */
    ScalarField2 generateStreamPowerField();

    /**
     * @brief generateIlluminationField Génère l'illuminationField du heightfield
     * @param nbSrcLum nombre de sources lumineuses tirées aléatoirement
     * @param nbPas nombre de pas testé pour chaque rayon
     * @return l'illuminationField du heightfield
     */
    ScalarField2 generateIlluminationField(int nbSrcLum = 30, int nbPas = 30);

    /**
     * @brief exportOBJ Réalise l'export du heighfield au format .obj
     * @param filename chemin du fichier à enregistrer
     * @param importNormals indique si l'on souhaite exporter les normales (par défaut oui)
     */
    void exportOBJ(const std::string & filename, bool importNormals = true);

    std::pair<int, int> initRay(Vector3 dir, Vector3 p);

    double zmin;
    double zmax;
};

#endif // HEIGHTFIELD_H
