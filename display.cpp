#include "display.h"
#include "ui_display.h"

Display::Display(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Display),
    showTree(false)
{
    ui->setupUi(this);
    ui->slopeImg->setBackgroundRole(QPalette::Base);
    ui->slopeImg->setScaledContents(true);
    ui->dareaImg->setBackgroundRole(QPalette::Base);
    ui->dareaImg->setScaledContents(true);
    ui->steamPowImg->setBackgroundRole(QPalette::Base);
    ui->steamPowImg->setScaledContents(true);
    ui->wetnessImg->setBackgroundRole(QPalette::Base);
    ui->wetnessImg->setScaledContents(true);
    ui->lightFieldImg->setBackgroundRole(QPalette::Base);
    ui->lightFieldImg->setScaledContents(true);
    ui->treeZoneImg->setBackgroundRole(QPalette::Base);
    ui->treeZoneImg->setScaledContents(true);
    ui->slopeLabel->setAlignment(Qt::AlignCenter);
    ui->dareaLabel->setAlignment(Qt::AlignCenter);
    ui->steamPowLabel->setAlignment(Qt::AlignCenter);
    ui->wetnessLabel->setAlignment(Qt::AlignCenter);
    ui->wetnessLabel->setStyleSheet("QLabel { background-color : black;}");
    ui->slopeLabel->setStyleSheet("QLabel { background-color : black;}");
    ui->dareaLabel->setStyleSheet("QLabel { background-color : black;}");
    ui->steamPowLabel->setStyleSheet("QLabel { background-color : black;}");

    connect(ui->showTrees, SIGNAL(clicked(bool)), this, SLOT(switchShowTrees()));
}

void Display::drawHFBase(DrawField & hf){
    ui->glview->setHFBase(hf);
}

void Display::setSlopeField(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->slopeImg->setPixmap(m);
}

void Display::setDrainageArea(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->dareaImg->setPixmap(m);
}

void Display::setStreamPower(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->steamPowImg->setPixmap(m);
}


void Display::setWetness(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->wetnessImg->setPixmap(m);
}

void Display::setLightField(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->lightFieldImg->setPixmap(m);
}

void Display::setTreeZones(QImage im){
    QPixmap m;
    m=m.fromImage(im);
    ui->treeZoneImg->setPixmap(m);
}


void Display::switchShowTrees(){
    if(showTree) showTree = false;
    else showTree = true;
    ui->glview->showTree = showTree;
    ui->glview->updateGL();
}


Display::~Display()
{
    delete ui;
}
