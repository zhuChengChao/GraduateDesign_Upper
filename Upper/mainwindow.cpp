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
    delete ui;
}

void MainWindow::AllIint()
{
    SearchAvailPort();
    InitSerialPort();

    //参数初始化
    BlurSize = 3;
    Thresh = 10;

    //定时器初始化
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(GetFrame()));
}

//测试按钮
void MainWindow::on_pushButton_clicked()
{
    //串口发送测试
    SendData();
}


