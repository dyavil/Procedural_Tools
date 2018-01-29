#include "display.h"
#include "ui_display.h"
#include <QTest>

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
    connect(ui->ErodeButton, SIGNAL(clicked(bool)), this, SLOT(erodeField()));
    ui->erodeSpinBox->setMinimum(1);
    ui->erodeSpinBox->setMaximum(1000);
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


void Display::erodeField(){
    Operation *op = new Operation(this, ui);
    std::cout << op->isWidgetType() << std::endl;

}


Display::~Display()
{
    delete ui;
}


Operation::Operation(QObject *parent, Ui::Display *ui)
    : QObject(parent), steps(0), ui(ui), start(false)
{
    int p = ui->erodeSpinBox->value();
    pd = new QProgressDialog("Eroding", "Cancel", 0, p+2);
    pd->setWindowTitle("Eroding");
    pd->setLabelText("Eroding init..");
    connect(pd, SIGNAL(canceled()), this, SLOT(cancel()));
    t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(perform()));


    t->start(0);
    pd->setVisible(true);

    //////////////////////////
    //init fields
    lf = LayerField(ui->glview->getDrawField()->fields);
    curHeight = lf.computeHeight();

    ///////////////////////////
}

void Operation::perform()
{
    pd->setValue(steps);
    //if(steps < 1) QTest::qSleep(100);
//    /std::cout << steps<<std::endl;
    if(steps > 0 && steps < pd->maximum()-1) {
        pd->setLabelText("Eroding step "+QString::number(steps));
        lf.generateThemralErosion(curHeight, 1, 10, 1, 40, 20, 20);
    }else if(steps == pd->maximum()-1){
        vegetationField veget = vegetationField(curHeight);
        slope = curHeight.generateSlopeField();
        imgs.push_back(slope.render());

        drain = curHeight.generateDrainageArea();
        imgs.push_back(drain.render());

        wetness = curHeight.generateWetnessField();
        imgs.push_back(wetness.render());

        stream = curHeight.generateStreamPowerField();
        imgs.push_back(stream.render());

        light = curHeight.generateIlluminationField();
        imgs.push_back(light.render());

        ScalarField2 vegetrep = veget.adaptVegetation(slope, wetness, light, stream);
        imgs.push_back(vegetrep.render());

        ui->glview->getDrawField()->setField(curHeight);
        ui->glview->getDrawField()->prepare();
        ui->glview->getDrawField()->addVeget(veget);
        ui->glview->getDrawField()->addRivers(drain);
    }
    steps++;

    if (steps > pd->maximum()){

        QPixmap m = m.fromImage(imgs[0]);
        ui->slopeImg->setPixmap(m);
        m = m.fromImage(imgs[1]);
        ui->dareaImg->setPixmap(m);
        m = m.fromImage(imgs[2]);
        ui->wetnessImg->setPixmap(m);
        m = m.fromImage(imgs[3]);
        ui->steamPowImg->setPixmap(m);
        m = m.fromImage(imgs[4]);
        ui->lightFieldImg->setPixmap(m);
        m = m.fromImage(imgs[5]);
        ui->treeZoneImg->setPixmap(m);
        t->stop();
        pd->setVisible(false);
        ui->glview->updateGL();
        delete pd;
        delete this;
    }
}

void Operation::cancel()
{
    t->stop();
    pd->setVisible(false);
    delete pd;
    delete this;
    //... cleanup
}

