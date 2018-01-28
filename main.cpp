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
    HeightField curHeight = lf.computeHeight();
    ScalarField2 slope, drain, wetness, stream, light;
    vegetationField veget = vegetationField(curHeight, 10.0);

    std::cout << "step0" <<std::endl;
    //lf.generateThemralErosion(curHeight, light, 1, 10, 1, 20, 20);

    slope = curHeight.generateSlopeField();
    w.setSlopeField(slope.render());
    if(renderImage) slope.render().save(QString(resdir) + QString("slope.png"));
    std::cout << "step1" <<std::endl;

    drain = curHeight.generateDrainageArea();
    w.setDrainageArea(drain.render());
    if(renderImage) drain.render().save(QString(resdir) + QString("drainageArea.png"));
    std::cout << "step1" <<std::endl;
    wetness = curHeight.generateWetnessField();
    w.setWetness(wetness.render());
    if(renderImage) wetness.render().save(QString(resdir) + QString("wetness.png"));
    std::cout << "step3" <<std::endl;
    stream = curHeight.generateStreamPowerField();
    w.setStreamPower(stream.render());
    if(renderImage) stream.render().save(QString(resdir) + QString("streamPower.png"));
    std::cout << "step4" <<std::endl;

    light = curHeight.generateIlluminationField();
    w.setLightField(light.render());
    if(renderImage) light.render().save(QString(resdir) + QString("lightField.png"));
    std::cout << "step5" <<std::endl;

    veget.render().save(QString(resdir) + QString("testpoissonprev.png"));
    ScalarField2 vegetview = veget.adaptVegetation(slope, wetness, light, stream);
    w.setTreeZones(vegetview.render());
    if(renderImage) vegetview.render().save(QString(resdir) + QString("veget.png"));
    std::cout << "step6" <<std::endl;

    DrawField d;
    d.setField(curHeight);
    std::cout << "step7" <<std::endl;
    d.prepare();
    std::cout << "step8" <<std::endl;
    d.loadTreeObj("lowpolytree3.obj");
    d.addVeget(veget);
    std::cout << "step9" <<std::endl;
    d.addRivers(drain);
    std::cout << "step10" <<std::endl;

    w.drawHFBase(d);
    std::cout << "step11" <<std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    HeightField hf = HeightField(Vector2(-2000, -2000), Vector2(2000, 2000), 512, 512, 480, 0);
    hf.load("heightmaps/map5.png");
    //hf.noiseMap(4, 1.0, 1994);
    LayerField lf = LayerField(hf);

    init(lf, w, true);

    return a.exec();
}
