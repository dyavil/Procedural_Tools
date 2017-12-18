#include "display.h"
#include <QApplication>
#include <QImageWriter>
#include "drawfield.h"
#include "heightfield.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    DrawField d;
    ScalarField2 hg = ScalarField2(Vector2(0, 0), Vector2(1, 1), 10, 10, 0.0);
    HeightField hf = HeightField(Vector2(-1, -1), Vector2(1, 1), 500, 500, 0.0);
    QImage im = QImage("/home/dyavil/Images/map1.png");
    //hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    hf.load(im, Vector2(-1, -1), Vector2(1, 1), 0.1, 0.5);
    //hf.noiseMap(4);

    hg = hf.generateSlopeField();
    d.setField(hf);
    //hf.exportOBJ("/home/dyavil/Images/map1.obj", false);
    //d.prepareInterpol(400);
    d.prepare();

    w.setSlopeField(hg.render());

    hg = hf.generateDrainageArea();
    d.addRivers(hg);
    w.setDrainageArea(hg.render());
    w.setWetness(hf.generateWetnessField().render());
    w.setStreamPower(hf.generateStreamPowerField().render());

    w.drawHFBase(d);
    hf.generateWetnessField().render().save("/home/dyavil/Images/res1.png");
    return a.exec();
}


/*
v2, v3
box2 : v2 a, b

*/
