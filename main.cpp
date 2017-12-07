#include "display.h"
#include <QApplication>
#include "drawfield.h"
#include "heightfield.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    DrawField d;
    ScalarField2 hg = ScalarField2(Vector2(0, 0), Vector2(1, 1), 10, 10, 0.0);
    HeightField hf = HeightField(Vector2(0, 0), Vector2(1, 1), 3, 3, 0.1);

    QImage im = QImage("/home/dyavil/Images/map1.png");
    hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    hf.load(im, Vector2(-1, -1), Vector2(1, 1), 0.1, 0.5);
    std::cout << hf.normal(1, 1) << std::endl;
    d.addField(hf);

    d.prepareInterpol(30, 0);
    w.drawHFBase(d);
    return a.exec();
}


/*
v2, v3
box2 : v2 a, b

*/
