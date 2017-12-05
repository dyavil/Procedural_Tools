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
    void drawHFBase(DrawField hf);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
