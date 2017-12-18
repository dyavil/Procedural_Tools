#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

#include <fstream>
#include <unordered_map>
#include "scalarfield2.h"
#include "vector"

class HeightField : public ScalarField2
{
public:
    HeightField() {}
    HeightField(Vector2 a, Vector2 b, int ww, int hh, double defaut=0.0) : ScalarField2(a, b, ww, hh, defaut) {}

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


    ScalarField2 generateWetnessField();


    ScalarField2 generateStreamPowerField();

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
     * @brief exportOBJ Réalise l'export du heighfield au format .obj
     * @param filename chemin du fichier à enregistrer
     * @param importNormals indique si l'on souhaite exporter les normales (par défaut oui)
     */
    void exportOBJ(const std::string & filename, bool importNormals = true);
};

#endif // HEIGHTFIELD_H
