#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>
#include <QFileDialog>

#include "drawfield.h"

namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    /**
     * @brief drawHFBase initialise l'interface
     * @param hf scene initiale
     */
    void drawHFBase(DrawField & hf);

    /**
     * @brief setSlopeField change l'image dans la fenetre de la slope
     * @param im image remplaçante
     */
    void setSlopeField(QImage im);

    /**
     * @brief setDrainageArea change l'image dans la fenetre de la drainage area
     * @param im image remplaçante
     */
    void setDrainageArea(QImage im);

    /**
     * @brief setStreamPower change l'image dans la fenetre du streampower
     * @param im image remplaçante
     */
    void setStreamPower(QImage im);

    /**
     * @brief setWetness change l'image dans la fenetre de la wetness
     * @param im image remplaçante
     */
    void setWetness(QImage im);

    /**
     * @brief setLightField change l'image dans la fenetre de l'illumiation
     * @param im image remplaçante
     */
    void setLightField(QImage im);

    /**
     * @brief setTreeZones change l'image dans la fenetre de la representation de la vegetation
     * @param im image remplaçante
     */
    void setTreeZones(QImage im);

    /**
     * @brief recompute recalcule l'affichage en fonction des parametres
     * @param hf base pour le calcul
     */
    void recompute(HeightField & hf);

    /**
     * @brief loading change l'affichage lors du chargement d'une map
     * @param hf carte du chargement
     */
    void loading(HeightField & hf);

private:
    Ui::Display *ui;
    bool showTree;

public slots:
    /**
     * @brief switchShowTrees affichage ou non de la vegetation
     */
    void switchShowTrees();

    /**
     * @brief erodeField boucle d'erosion
     */
    void erodeField();

    /**
     * @brief loadMap action de charger une map
     *          depuis une image avec les parametres
     *          renseigné dans l'interface
     */
    void loadMap();

    /**
     * @brief noiseMap action de créer une map
     *          de somme de bruit avec les parametres
     *          renseigné dans l'interface
     */
    void noiseMap();

    /**
     * @brief exportObj gere le pop up d'export
     */
    void exportObj();

};


/**
 * @brief The Operation class
 *  Gere l'affichage de la progression de l'erosion
 */
class Operation: public QObject
{
    Q_OBJECT
public:
    explicit Operation(QObject *parent = 0, Ui::Display *ui = 0);

signals:

public slots:
    void perform();
    void cancel();
private:
    int steps;
    QProgressDialog *pd;
    Ui::Display *ui;
    QTimer *t;
    bool start;
    std::vector<QImage> imgs;

    ScalarField2 slope, drain, wetness, stream, light;
    LayerField lf;
    HeightField curHeight;

};

#endif // DISPLAY_H
