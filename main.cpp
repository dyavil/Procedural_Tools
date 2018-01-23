#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include "drawfield.h"
#include "layerfield.h"
#include "vegetationfield.h"

#define resdir "/media/emeric/DATA/Documents/Etudes/M2/Procedural/TP/images/"
// /media/emeric/DATA/Documents/Etudes/M2/Procedural/TP/images/
// /home/dyavil/Images/


void init(LayerField &lf, Display &w, bool renderImage = false) {
    HeightField curHeight = lf.computeHeight();
    ScalarField2 slope, drain, wetness, stream, light;
    vegetationField veget = vegetationField(Vector2(-2000, -2000), Vector2(2000, 2000), 0.0, 20.0);

    /*
    slope = curHeight.generateSlopeField();
    w.setSlopeField(slope.render());
    if(renderImage) slope.render().save(QString(resdir) + QString("slope.png"));

    drain = curHeight.generateDrainageArea();
    d.addRivers(drain);
    w.setDrainageArea(drain.render());
    if(renderImage) drain.render().save(QString(resdir) + QString("drainageArea.png"));

    wetness = curHeight.generateWetnessField();
    w.setWetness(wetness.render());
    if(renderImage) wetness.render().save(QString(resdir) + QString("wetness.png"));

    stream = curHeight.generateStreamPowerField();
    w.setStreamPower(stream.render());
    if(renderImage) stream.render().save(QString(resdir) + QString("streamPower.png"));

    light = curHeight.generateIlluminationField();
    w.setLightField(light.render());
    if(renderImage) light.render().save(QString(resdir) + QString("lightField.png"));

    veget.render().save(QString(resdir) + QString("testpoissonprev.png"));
    veget.adaptVegetation(currentHeight);
    veget.render().save(QString(resdir) + QString("testpoisson.png"));
    d.addVeget(veget);
    */

    lf.generateThemralErosion(curHeight, light, 50, 10, 1, 20, 20, true);

    DrawField d;
    d.setField(curHeight);
    d.prepare();

    w.drawHFBase(d);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    HeightField hf = HeightField(Vector2(-2000, -2000), Vector2(2000, 2000), 512, 512, 600, 0);
    hf.load("heightmaps/map5.png");
    //hf.noiseMap(4);
    LayerField lf = LayerField(hf);

    init(lf, w, true);

    return a.exec();
}
