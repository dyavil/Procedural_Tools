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
    ui->erodeSpinBox->setMaximum(2000);
    ui->ratioBox->setMaximum(6);
    ui->ratioBox->setMinimum(1);
    ui->maxHeightBox->setMinimum(10.0);
    ui->maxHeightBox->setMaximum(3000.0);
    ui->seedBox->setMinimum(0);
    ui->seedBox->setMaximum(8000);
    ui->seedBox->setValue(125);
    ui->sizeBox->setMinimum(200.0);
    ui->sizeBox->setMaximum(10000.0);
    ui->sizeNoiseBox->setMinimum(1000.0);
    ui->sizeNoiseBox->setMaximum(10000.0);
    ui->octavesBox->setMinimum(1);
    ui->octavesBox->setValue(3);
    connect(ui->loadMapButton, SIGNAL(released()), this, SLOT(loadMap()));
    connect(ui->renderNoiseButton, SIGNAL(released()), this, SLOT(noiseMap()));
    connect(ui->exportObjBtn, SIGNAL(released()), this, SLOT(exportObj()));
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

void Display::loadMap() {
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Ouvrir image"), "",
            tr("Jpeg (*.jpg);;Png (*.png);;All Files (*)"));
    if(!fileName.isEmpty()){
        double minX = -(ui->sizeBox->value()/2.0);
        double maxX = (ui->sizeBox->value()/2.0);
        double heigthMax = ui->maxHeightBox->value();
        HeightField hf = HeightField(Vector2(minX, minX), Vector2(maxX, maxX), 512, 512, heigthMax, 0);
        hf.load("heightmaps/load.png");
        loading(hf);
        hf.load(fileName);
        recompute(hf);
    }
}

void Display::loading(HeightField & hf){
    DrawField d;
    LayerField lf = LayerField(hf);
    d.setField(lf);
    d.prepare();
    ui->glview->setHFBase(d);
    ui->glview->updateGL();
}

void Display::noiseMap(){
    double minX = -(ui->sizeNoiseBox->value()/2.0);
    double maxX = (ui->sizeNoiseBox->value()/2.0);
    HeightField hf = HeightField(Vector2(minX, minX), Vector2(maxX, maxX), 512, 512, 500, 0);
    hf.noiseMap(ui->octavesBox->value(), ui->ratioBox->value(), ui->seedBox->value());
    recompute(hf);
}

void Display::recompute(HeightField & hf){
    DrawField d;
    ScalarField2 slope, drain, wetness, stream, light;
    LayerField lf = LayerField(hf);
    HeightField curHeight = lf.computeHeight();
    slope = curHeight.generateSlopeField();
    std::cout << "Loading.." << std::endl;

    drain = curHeight.generateDrainageArea();

    wetness = curHeight.generateWetnessField();

    stream = curHeight.generateStreamPowerField();

    light = curHeight.generateIlluminationField();
    vegetationField veget = vegetationField(curHeight, slope, wetness, light, stream);

    QPixmap m = m.fromImage(slope.render());
    ui->slopeImg->setPixmap(m);
    m = m.fromImage(drain.render());
    ui->dareaImg->setPixmap(m);
    m = m.fromImage(wetness.render());
    ui->wetnessImg->setPixmap(m);
    m = m.fromImage(stream.render());
    ui->steamPowImg->setPixmap(m);
    m = m.fromImage(light.render());
    ui->lightFieldImg->setPixmap(m);
    m = m.fromImage(veget.genImage().render());
    ui->treeZoneImg->setPixmap(m);

    d.setField(lf);
    d.prepare();
    for (unsigned int i = 0; i < trees.size(); ++i) {
        d.loadTreeObj(trees[i].objPath, i);
    }
    d.addVeget(veget);
    d.addRivers(drain);
    ui->glview->setHFBase(d);
    ui->glview->updateGL();
}

void Display::exportObj(){
    QString fileName = QFileDialog::getSaveFileName(this,
                        tr("Export obj"), "",
                        tr("Obj (*.obj);;All Files (*)"));
    if(!fileName.isEmpty()){
        ui->glview->getDrawField()->fields.exportOBJ(fileName.toStdString());
    }
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
    lf = ui->glview->getDrawField()->layerf;
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

        vegetationField veget = vegetationField(curHeight, slope, wetness, light, stream);
        imgs.push_back(veget.genImage().render());

        ui->glview->getDrawField()->setField(lf);
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

