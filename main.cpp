#include "display.h"
#include <QApplication>
#include "scalarfiled2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    ScalarField2 hg = ScalarField2(Vector2(1, 0), Vector2(0, 1), 10, 10, 0.0);
    QImage im = QImage("/home/dyavil/Images/map1.png");
    hg.load(im, Vector2(-1, -1), Vector2(1, 1), 0.3, 0.6);
    hg.inside(Vector3(0.95, 0.095, 0.0));
    w.drawHFBase(hg);
    return a.exec();
}


/*
v2, v3
box2 : v2 a, b

*/
