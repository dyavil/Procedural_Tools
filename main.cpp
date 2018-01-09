#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include "drawfield.h"
#include "heightfield.h"

#define resdir "/home/dyavil/Images/"


void init(HeightField &hf, Display &w, int nbrayon, bool renderImage=false){
    DrawField d;
    d.setField(hf);
    d.prepare();
    ScalarField2 tmp = hf.generateSlopeField();
    w.setSlopeField(tmp.render());
    if(renderImage) tmp.render().save(QString(resdir) + QString("slope.png"));

    tmp = hf.generateDrainageArea();
    d.addRivers(tmp);
    w.setDrainageArea(tmp.render());
    if(renderImage) tmp.render().save(QString(resdir) + QString("drainageArea.png"));

    tmp = hf.generateWetnessField();
    w.setWetness(tmp.render());
    if(renderImage) tmp.render().save(QString(resdir) + QString("wetness.png"));

    tmp = hf.generateStreamPowerField();
    w.setStreamPower(tmp.render());
    if(renderImage) tmp.render().save(QString(resdir) + QString("streamPower.png"));

    tmp = hf.generateIlluminationField(nbrayon);
    w.setLightField(tmp.render());
    if(renderImage) tmp.render().save(QString(resdir) + QString("lightField.png"));
    w.drawHFBase(d);
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();
    HeightField hf = HeightField(Vector2(-4000, -4000), Vector2(4000, 4000), 500, 500, 0.0);
    QImage im = QImage("heightmaps/map8.png");
    //hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    hf.load(im, Vector2(-2000, -2000), Vector2(2000, 2000), 0, 600);
    //hf.noiseMap(4);
    init(hf, w, 20, true);

    return a.exec();

}

/*
v2, v3
box2 : v2 a, b

*/
