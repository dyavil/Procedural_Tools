#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
}

void Display::drawHFBase(DrawField hf){
    ui->glview->setHFBase(hf);
}

Display::~Display()
{
    delete ui;
}
