#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "params.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//测试按钮
void MainWindow::on_pushButton_clicked()
{
    //串口发送测试
    SendData();
}

