#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include <QProgressDialog>
#include <QTimer>
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
    void setTreeZones(QImage im);
private:
    Ui::Display *ui;
    bool showTree;

public slots:
    void switchShowTrees();
    void erodeField();

};

class Operation: public QObject
{
    Q_OBJECT
public:
    explicit Operation(QObject *parent = 0, Ui::Display *ui = 0);

signals:

public slots:
    void perform();
    void cancel();
private:
    int steps;
    QProgressDialog *pd;
    Ui::Display *ui;
    QTimer *t;
    bool start;
    std::vector<QImage> imgs;

    ScalarField2 slope, drain, wetness, stream, light;
    LayerField lf;
    HeightField curHeight;

};

#endif // DISPLAY_H
