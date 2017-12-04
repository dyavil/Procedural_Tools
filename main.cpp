#include "display.h"
#include <QApplication>
#include "scalarfiled2.h"
#include "heightfield.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    ScalarField2 hg = ScalarField2(Vector2(0, 0), Vector2(1, 1), 10, 10, 0.0);
    HeightField hf = HeightField(Vector2(0, 0), Vector2(1, 1), 10, 10, 0.0);
    QImage im = QImage("/home/dyavil/Images/map1.png");
    hg.load(im, Vector2(-0.5, -0.5), Vector2(0.5, 0.5), 0.3, 0.6);
    hf.load(im, Vector2(-0.5, -0.5), Vector2(0.5, 0.5), 0.3, 0.6);
    std::cout << hf.normal(1, 1) << std::endl;
    double res;
    hf.Bilineaire(Vector2(0.455, 0.4), res);
    std::cout << res << std::endl;
    hg.inside(Vector3(0.95, 0.095, 0.0));
    w.drawHFBase(hg);
    return a.exec();
}


/*
v2, v3
box2 : v2 a, b

*/
