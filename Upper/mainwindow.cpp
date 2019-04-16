#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "params.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    AllIint();
}

MainWindow::~MainWindow()
{
    Capture.release();
    delete ui;
}

void MainWindow::AllIint()
{
    SearchAvailPort();
    InitSerialPort();

    //参数初始化
    BlurSize = 3;
    ui->spinBox_BlurSize->setValue(BlurSize);
    Thresh = 5;
    ui->spinBox_ThresValue->setValue(Thresh);
    MinBlobArea = 5;
    ui->spinBox_MinBlibSize->setValue(MinBlobArea);

    //定时器初始化
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(GetFrame()));
}


//二值化阈值调节
void MainWindow::on_spinBox_ThresValue_valueChanged(int arg1)
{
    Thresh = arg1;
    //qDebug()<<Thresh;
}


//滤波器的尺寸调节
void MainWindow::on_spinBox_BlurSize_valueChanged(int arg1)
{
    BlurSize = arg1;
}


//面积最小值调节
void MainWindow::on_spinBox_MinBlibSize_valueChanged(int arg1)
{
    MinBlobArea = arg1;
}
