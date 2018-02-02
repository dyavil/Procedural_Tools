#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include <QDir>
#include "drawfield.h"
#include "layerfield.h"
#include "vegetationfield.h"


void init(LayerField &lf, Display &w, bool renderImage = false) {
    HeightField curHeight = lf.computeHeight();
    ScalarField2 slope, drain, wetness, stream, light;

    curHeight.render().save(QString("maps/heightmap.png"));
    std::cout << "step0" <<std::endl;
    //lf.generateThemralErosion(curHeight, light, 1, 10, 1, 20, 20);

    slope = curHeight.generateSlopeField();
    w.setSlopeField(slope.render());
    if(renderImage) slope.render().save(QString("maps/slope.png"));
    std::cout << "step1" <<std::endl;

    drain = curHeight.generateDrainageArea();
    w.setDrainageArea(drain.render());
    if(renderImage) drain.render().save(QString("maps/drainageArea.png"));
    std::cout << "step2" <<std::endl;
    wetness = curHeight.generateWetnessField();
    w.setWetness(wetness.render());
    if(renderImage) wetness.render().save(QString("maps/wetness.png"));
    std::cout << "step3" <<std::endl;
    stream = curHeight.generateStreamPowerField();
    w.setStreamPower(stream.render());
    if(renderImage) stream.render().save(QString("maps/streamPower.png"));
    std::cout << "step4" <<std::endl;

    light = curHeight.generateIlluminationField();
    w.setLightField(light.render());
    if(renderImage) light.render().save(QString("maps/lightField.png"));
    std::cout << "step5" <<std::endl;

    vegetationField veget = vegetationField(curHeight, slope, wetness, light, stream);
    //ScalarField2 vegetview = veget.adaptVegetation(slope, wetness, light, stream);
    std::cout << "step16" <<std::endl;
    w.setTreeZones(veget.genImage().render());
    if(renderImage) veget.genImage().render().save(QString("maps/veget.png"));
    std::cout << "step6" <<std::endl;

    DrawField d;
    d.setField(lf);
    std::cout << "step7" <<std::endl;
    d.prepare();
    std::cout << "step8" <<std::endl;
    d.loadTreeObj(trees[0].objPath);
    d.loadTreeObj(trees[1].objPath, 1);
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

    HeightField hf = HeightField(Vector2(-2000, -2000), Vector2(2000, 2000), 512, 512, 580, 0);
    hf.load("heightmaps/map5.png");
    //hf.noiseMap(4, 1.0, 1994);
    LayerField lf = LayerField(hf);

    init(lf, w, true);

    return a.exec();
}
