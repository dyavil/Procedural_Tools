#include "layerfield.h"

LayerField::LayerField(HeightField c1) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
    couche3 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
}

LayerField::LayerField(HeightField c1, ScalarField2 c2) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = c2;
    couche3 = ScalarField2(c1.a, c1.b, c1.w, c1.h);
}

LayerField::LayerField(HeightField c1, ScalarField2 c2, ScalarField2 c3) : Array2(c1.a, c1.b, c1.w, c1.h) {
    couche1 = c1;
    couche2 = c2;
    couche3 = c3;
}


HeightField LayerField::computeHeight(int nbCouches) {
    HeightField res(couche1.a, couche1.b, couche1.w, couche1.h, couche1.zmax, couche1.zmin);

    switch(nbCouches) {
        case 1:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)];
                }
            }
        case 2:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)] + couche2.field[pos(i, j)];
                }
            }
        case 3:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = couche1.field[pos(i, j)] + couche2.field[pos(i, j)] + couche3.field[pos(i, j)];
                }
            }
            break;
        default:
            for (int i = 0; i < h; ++i) {
                for (int j = 0; j < w; ++j) {
                    res.field[pos(i, j)] = 0;
                }
            }
            break;
    }

    return res;
}


void LayerField::generateThemralStress(ScalarField2 & light, double eroMax, int nbSrcLum, int nbPas) {
    HeightField heightMap = computeHeight();
    light = heightMap.generateIlluminationField(nbSrcLum, nbPas);
    ScalarField2 illumFieldNorm = light.sfNormalize();

    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            double qteErosion = std::max(0.0, eroMax * illumFieldNorm.field[pos(i, j)] - couche2.field[pos(i,j)]);
            couche1.field[pos(i, j)] -= qteErosion;
            couche2.field[pos(i, j)] += qteErosion;
        }
    }
}


void LayerField::updateNeighborsSediment(int position, double qteTransport, double angleMin) {
    Vector2 vec = coord(position);
    int i = vec.x, j = vec.y;
    double coeff[8] = {}, somCoeff = 0;
    double curHeight = couche1.field[position] + couche2.field[position];

    // Calcul des coefficients en fonction de la pente
    if((i-1) >= 0 && (j+1) < w && couche1.field[pos(i-1, j+1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i-1, j+1)] + couche2.field[pos(i-1, j+1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan((curHeight - nbrHeight) / M_SQRT2);
            if(curAngle >= angleMin) {
                coeff[0] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((j+1) < w && couche1.field[pos(i, j+1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i, j+1)] + couche2.field[pos(i, j+1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan(curHeight - nbrHeight);
            if(curAngle >= angleMin) {
                coeff[1] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((i+1) < h && (j+1) < w && couche1.field[pos(i+1, j+1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i+1, j+1)] + couche2.field[pos(i+1, j+1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan((curHeight - nbrHeight) / M_SQRT2);
            if(curAngle >= angleMin) {
                coeff[2] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((i+1) < h && couche1.field[pos(i+1, j)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i+1, j)] + couche2.field[pos(i+1, j)];
        if(nbrHeight < curHeight) {
            double curAngle = atan(curHeight - nbrHeight);
            if(curAngle >= angleMin) {
                coeff[3] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((i+1) < h && (j-1) >= 0 && couche1.field[pos(i+1, j-1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i+1, j-1)] + couche2.field[pos(i+1, j-1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan((curHeight - nbrHeight) / M_SQRT2);
            if(curAngle >= angleMin) {
                coeff[4] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((i-1) >= 0 && couche1.field[pos(i-1, j)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i-1, j)] + couche2.field[pos(i-1, j)];
        if(nbrHeight < curHeight) {
            double curAngle = atan(curHeight - nbrHeight);
            if(curAngle >= angleMin) {
                coeff[5] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((i-1) >= 0 && (j-1) >= 0 && couche1.field[pos(i-1, j-1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i-1, j-1)] + couche2.field[pos(i-1, j-1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan((curHeight - nbrHeight) / M_SQRT2);
            if(curAngle >= angleMin) {
                coeff[6] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    if((j-1) >= 0 && couche1.field[pos(i, j-1)] < couche1.field[position]) {
        double nbrHeight = couche1.field[pos(i, j-1)] + couche2.field[pos(i, j-1)];
        if(nbrHeight < curHeight) {
            double curAngle = atan(curHeight - nbrHeight);
            if(curAngle >= angleMin) {
                coeff[7] = curAngle;
                somCoeff += curAngle;
            }
        }
    }

    // On distribue notre sédiment aléatoirement sur une pente
    std::uniform_real_distribution<double> distrib(0.0, somCoeff);
    std::default_random_engine random;
    double rand = distrib(random);

    for(int i = 0; i < 8; ++i) {
        rand -= coeff[i];
        if(rand <= 0) {
            double qteEro = std::max(0.0, couche2.field[position] - qteTransport);
            couche2.field[position] -= qteEro;

            switch(i) {
                case 0:
                    couche2.field[pos(i-1, j+1)] += qteEro;
                    break;
                case 1:
                    couche2.field[pos(i, j+1)] += qteEro;
                    break;
                case 2:
                    couche2.field[pos(i+1, j+1)] += qteEro;
                    break;
                case 3:
                    couche2.field[pos(i+1, j)] += qteEro;
                    break;
                case 4:
                    couche2.field[pos(i+1, j-1)] += qteEro;
                    break;
                case 5:
                    couche2.field[pos(i-1, j)] += qteEro;
                    break;
                case 6:
                    couche2.field[pos(i-1, j-1)] += qteEro;
                    break;
                case 7:
                    couche2.field[pos(i, j-1)] += qteEro;
                    break;
            }
            return;
        }
    }
}


void LayerField::sedimentTransport(unsigned int nbIters, double qteTransport, double angleMin) {
    std::vector<std::array<double, 2>> vecHeights(couche1.field.size());

    for(unsigned int k = 0; k < nbIters; ++k) {

        for(int i = 0; i < h; ++i) {
            for(int j = 0; j < w; ++j) {
                int position = pos(i,j);
                double height = couche1.field[position] + couche2.field[position];
                vecHeights[position] = { height , (double)position };
            }
        }

        std::sort(vecHeights.rbegin(), vecHeights.rend());

        for(unsigned int i = 0; i < vecHeights.size(); ++i) {
            if(couche2.field[i] >= qteTransport) {
                updateNeighborsSediment(vecHeights[i][1], qteTransport, angleMin);
            }
        }
    }
}


void LayerField::generateThemralErosion(HeightField & hf, ScalarField2 & light, int nbSimu,
                                        double eroMax, double qteSedTrans, int nbSrcLum, int nbPasLum, bool saveImg) {

    for(int i = 1; i <= nbSimu; ++i) {
        std::cout << "Erosion - passe n°" << i << std::endl;

        generateThemralStress(light, eroMax, nbSrcLum, nbPasLum);
        sedimentTransport(eroMax/qteSedTrans, qteSedTrans);

        if(saveImg) {
            QString path = QString("/media/emeric/DATA/Documents/Etudes/M2/Procedural/TP/images/");
            computeHeight().render().save(path + QString("ero_") + QString::number(i) + QString(".png"));
        }
    }

    hf = computeHeight();
}


void LayerField::setVegetField(ScalarField2 &veget){
    vegetation = veget;
}
