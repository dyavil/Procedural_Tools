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

    hf.setVal(1, 1, 0.3);
    QImage im = QImage("/home/dyavil/Images/map3.jpg");
    hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    hf.load(im, Vector2(-1, -1), Vector2(1, 1), 0.1, 0.8);
    std::cout << hf.normal(1, 1) << std::endl;
    double res;
    hf.Bilineaire(Vector2(0.455, 0.4), res);
    std::cout << res << std::endl;
    hg.inside(Vector3(0.95, 0.095, 0.0));
    d.addField(hf);
    w.drawHFBase(d);
    return a.exec();
}


/*
v2, v3
box2 : v2 a, b

*/
