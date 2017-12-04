#ifndef DISPLAY_H
#define DISPLAY_H

#include <QMainWindow>
#include "scalarfiled2.h"

namespace Ui {
class Display;
}

class Display : public QMainWindow
{
    Q_OBJECT

public:
    explicit Display(QWidget *parent = 0);
    ~Display();
    void drawHFBase(ScalarField2 hf);

private:
    Ui::Display *ui;
};

#endif // DISPLAY_H
