#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include "drawfield.h"
#include "layerfield.h"
#include "vegetationfield.h"

#define resdir "/home/dyavil/Images/"
// /media/emeric/DATA/Documents/Etudes/M2/Procedural/TP/images/
// /home/dyavil/Images/


void init(LayerField &lf, Display &w, bool renderImage = false) {
    HeightField currentHeight = lf.computeHeight();
    ScalarField2 slope, drain, wetness, stream, illum;
    vegetationField veget = vegetationField(currentHeight, 20.0);

    DrawField d;
    d.setField(currentHeight);
    d.prepare();
    d.loadTreeObj("lowpolytree3.obj");

    slope = currentHeight.generateSlopeField();
    w.setSlopeField(slope.render());
    if(renderImage) slope.render().save(QString(resdir) + QString("slope.png"));

    drain = currentHeight.generateDrainageArea();
    d.addRivers(drain);
    w.setDrainageArea(drain.render());
    if(renderImage) drain.render().save(QString(resdir) + QString("drainageArea.png"));

    wetness = currentHeight.generateWetnessField(drain, slope);
    w.setWetness(wetness.render());
    if(renderImage) wetness.render().save(QString(resdir) + QString("wetness.png"));

    stream = currentHeight.generateStreamPowerField(drain, slope);
    w.setStreamPower(stream.render());
    if(renderImage) stream.render().save(QString(resdir) + QString("streamPower.png"));

    illum = currentHeight.generateIlluminationField();
    w.setLightField(illum.render());
    if(renderImage) illum.render().save(QString(resdir) + QString("lightField.png"));

    veget.render().save(QString(resdir) + QString("testpoissonprev.png"));
    ScalarField2 vegetview = veget.adaptVegetation(slope, wetness, illum, stream);
    w.setTreeZones(vegetview.render());
    if(renderImage) vegetview.render().save(QString(resdir) + QString("veget.png"));
    d.addVeget(veget);

    // BUG HERE !
    //lf.generateThemralErosion(1);

    w.drawHFBase(d);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    HeightField hf = HeightField(Vector2(-2000, -2000), Vector2(2000, 2000), 512, 512, 500, 0);
    hf.load("heightmaps/map5.png");
    hf.noiseMap(4);
    LayerField lf = LayerField(hf);

    init(lf, w, true);

    return a.exec();
}
