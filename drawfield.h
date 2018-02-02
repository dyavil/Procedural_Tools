#ifndef DRAWFIELD_H
#define DRAWFIELD_H

#include <vector>
#include <GL/gl.h>
#include "heightfield.h"
#include "layerfield.h"
#include "vegetationfield.h"
#include "include/tiny_obj_loader.h"

class DrawField
{
public:
    DrawField();
    /**
     * @brief prepare initialise les attributs en fonction de fields
     */
    void prepare();

    /**
     * @brief addRivers ajoute l'affichage du stream power sur la carte
     * @param sf field de streampower
     */
    void addRivers(const ScalarField2 & sf);

    /**
     * @brief addVeget ajoute la vegetation a l'affichage
     * @param sf field de vegetation
     */
    void addVeget(vegetationField & sf);
    /**
     * @brief draw dessine en gl le terrain
     * @param showTree vrai si affichage désiré de la vegetation
     */
    void draw(bool showTree);

    /**
     * @brief setField
     * @param lf
     */
    void setField(LayerField lf) {layerf = lf; fields=lf.computeHeight();}

    /**
     * @brief loadTreeObj assigne au arbre une forme
     * @param path du fichier obj
     * @param pos index du type d'arbre
     */
    void loadTreeObj(QString path, int pos = 0);

    LayerField layerf;
    HeightField fields;
    std::vector<Vector3> colors;
    std::vector<Vector3> vertices;
    std::vector<Triangle> triangles;
    std::vector<std::vector<Vector3>> treeVertices;
    std::vector<std::vector<Vector3>> treeColors;
    std::vector<std::vector<Vector3>> treeTranslations;
    std::vector<double> larg;


};

#endif // DRAWFIELD_H
