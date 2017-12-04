#include "display.h"
#include <QApplication>
#include "heightfield.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Display w;
    w.show();

    HeightField hg = HeightField(Vector2(1, 0), Vector2(0, 1), 10, 10, 0.0);
    QImage im = QImage("/home/dyavil/Images/map1.png");
    hg.load(im, Vector2(1, 0), Vector2(0, 1), 0, 1);
    w.drawHFBase(hg);
    return a.exec();
}
