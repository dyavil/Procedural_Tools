#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include "drawfield.h"
#include "layerfield.h"
#include "vegetationfield.h"

#define resdir "/home/dyavil/Images/"
// /media/emeric/DATA/Documents/Etudes/M2/Procedural/TP/images/
// /home/dyavil/Images/


void init(HeightField &hf, Display &w, int nbrayon, bool renderImage=false){
    DrawField d;
    d.setField(hf);
    d.prepare();
    ScalarField2 slope, drain, wet, stream, illum, ero;
    LayerField lf(hf);
    HeightField currentHeight = lf.computeHeight();

    slope = currentHeight.generateSlopeField();
    w.setSlopeField(slope.render());
    if(renderImage) slope.render().save(QString(resdir) + QString("slope.png"));

    drain = currentHeight.generateDrainageArea();
    d.addRivers(drain);
    w.setDrainageArea(drain.render());
    if(renderImage) drain.render().save(QString(resdir) + QString("drainageArea.png"));

    wet = currentHeight.generateWetnessField();
    w.setWetness(wet.render());
    if(renderImage) wet.render().save(QString(resdir) + QString("wetness.png"));

    stream = currentHeight.generateStreamPowerField();
    w.setStreamPower(stream.render());
    if(renderImage) stream.render().save(QString(resdir) + QString("streamPower.png"));

    illum = currentHeight.generateIlluminationField(nbrayon);
    w.setLightField(illum.render());
    if(renderImage) illum.render().save(QString(resdir) + QString("lightField.png"));

    ero = lf.generateThemralStress(illum, 10);
    w.setLightField(ero.render());
    if(renderImage) ero.render().save(QString(resdir) + QString("thermalErosion.png"));

    vegetationField veget = vegetationField(hf, 15.0);
    veget.render().save(QString(resdir) + QString("testpoissonprev.png"));
    ScalarField2 vegetview = veget.adaptVegetation(hf);
    w.setTreeZones(vegetview.render());
    if(renderImage) vegetview.render().save(QString(resdir) + QString("veget.png"));
    d.addVeget(veget);

    w.drawHFBase(d);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();
    HeightField hf = HeightField(Vector2(-10000, -10000), Vector2(10000, 10000), 500, 500, 0.0);
    QImage im = QImage("heightmaps/map9.png");
    hf.load(im, Vector2(-2000, -2000), Vector2(2000, 2000), 0, 500);
    init(hf, w, 20, true);

    //hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    //hf.noiseMap(4);

    return a.exec();
}
