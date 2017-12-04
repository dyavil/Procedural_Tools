#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
}

void Display::drawHFBase(HeightField hf){
    ui->glview->setHFBase(hf);
}

Display::~Display()
{
    delete ui;
}
