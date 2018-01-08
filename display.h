#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include "drawfield.h"

namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    void drawHFBase(DrawField & hf);
    void setSlopeField(QImage im);
    void setDrainageArea(QImage im);
    void setStreamPower(QImage im);
    void setWetness(QImage im);
    void setLightField(QImage im);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
