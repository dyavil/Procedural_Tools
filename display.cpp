#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display)
{
    ui->setupUi(this);
    ui->slopeImg->setBackgroundRole(QPalette::Base);
    ui->slopeImg->setScaledContents(true);
    ui->slopeLabel->setAlignment(Qt::AlignCenter);
}

void Display::drawHFBase(DrawField hf){
    ui->glview->setHFBase(hf);
}

void Display::setSlopeField(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->slopeImg->setPixmap(m);
}

Display::~Display()
{
    delete ui;
}
