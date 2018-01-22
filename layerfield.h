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


    void generateThemralStress(double eroMax, int nbSrcLum = 30, int nbPas = 30);

    void updateNeighborsSediment(int position, double qteTransport, double angleMin);

    void sedimentTransport(double qteTransport = 1, unsigned int nbIters = 10, double angleMin = 40);

    void generateThemralErosion(int nbSimu, double eroMax = 10, double qteSedTrans = 1, int nbSrcLum = 30, int nbPasLum = 30);


    void setVegetField(ScalarField2 & veget);

    HeightField couche1;
    ScalarField2 couche2;
    ScalarField2 couche3;
    ScalarField2 vegetation;
};

#endif // LAYERFIELD2_H
