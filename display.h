#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include "heightfield.h"

namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    void drawHFBase(HeightField hf);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
