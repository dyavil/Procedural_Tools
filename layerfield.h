#ifndef LAYERFIELD2_H
#define LAYERFIELD2_H

#include "heightfield.h"

class LayerField : public Array2
{
public:
    LayerField() {}
    LayerField(HeightField c1);
    LayerField(HeightField c1, ScalarField2 c2);
    LayerField(HeightField c1, ScalarField2 c2, ScalarField2 c3);

    /**
     * @brief computeHeight renvoie une HeightMap calculant la somme des couches
     * @param nbCouches nb de couches a sommer (par défaut les 2 premières)
     * @return la heightmap courante du layerfield
     */
    HeightField computeHeight(int nbCouches = 2);

    /**
     * @brief generateThemralStress transforme la bedrock (couche1) en sediment (couche2)
     * @param light map d'illumination qui sera mise à jour par cette fonction
     * @param eroMax quantité max (en m) de bedrock trasnformée en sediment
     * @param nbSrcLum nombre de sources de lumières utilisées pour la simu
     * @param nbPas double du nombre de pas effectués lors de la marche
     */
    void generateThemralStress(ScalarField2 & light, double eroMax, int nbSrcLum = 30, int nbPas = 30);

    /**
     * @brief updateNeighborsSediment étale la quantité de sédiments sur les positions voisines
     * @param position position du point qui est mis à jour
     * @param qteTransport quantité minimale de sédiment transportée
     * @param angleMin angle min au-delà duquel les sédiments s'écoulent
     */
    void updateNeighborsSediment(int position, double qteTransport, double angleMin);

    /**
     * @brief sedimentTransport étale les sédiments du layerfield
     * @param nbIters nombre d'itérations qu'on souhaite réaliser
     * @param qteTransport quantité minimale de sédiment transportée
     * @param angleMin angle min au-delà duquel les sédiments s'écoulent
     */
    void sedimentTransport(unsigned int nbIters, double qteTransport = 1, double angleMin = 40);

    /**
     * @brief generateThemralErosion simulation de l'érosion au cours du temps
     * @param hf heightmap mise à jour par cette fonction
     * @param light map d'illumination qui sera mise à jour
     * @param nbSimu nombre d'itérations souhaitées pour la simulation
     * @param eroMax quantité max (en m) de bedrock trasnformée en sediment
     * @param qteSedTrans quantité minimale de sédiment transportée
     * @param nbSrcLum nombre de sources de lumières utilisées pour la simu
     * @param nbPasLum double du nombre de pas effectués lors de la marche
     * @param saveImg indique si l'on enregistre les img pour chaque itération
     */
    void generateThemralErosion(HeightField & hf, ScalarField2 & light, unsigned int nbSimu,
                                double eroMax = 10, double qteSedTrans = 1, int nbSrcLum = 30, int nbPasLum = 30, bool saveImg = false);

    void setVegetField(ScalarField2 & veget);

    HeightField couche1;
    ScalarField2 couche2;
    ScalarField2 couche3;
    ScalarField2 vegetation;
};

#endif // LAYERFIELD2_H
