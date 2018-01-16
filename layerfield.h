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
     * @brief generateThemralErosion Génère la quantité de roche érodée par le soleil
     * @param illumField scalarField de l'illumination (non normalisé)
     * @param eroMax erosion maximum (en m)
     * @return la quantité de roche érodée du heightfield
     */
    ScalarField2 generateThemralStress(ScalarField2 & illumField, double eroMax);


    ScalarField2 sedimentTransport(ScalarField2 & sedField, double angleMin);


    ScalarField2 generateThemralErosion(ScalarField2 & illumField, int nbSimu);


    void setVegetField(ScalarField2 & veget);

    HeightField couche1;
    ScalarField2 couche2;
    ScalarField2 couche3;
    ScalarField2 vegetation;
};

#endif // LAYERFIELD2_H
