#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "params.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    //串口相关函数
    void ReceiveData();
    void SendData();
    void on_pushButton_searchPort_clicked();
    void on_pushButton_openPort_clicked();

    void on_pushButton_clicked();

private:
    //函数定义：
    //串口相关函数：
    void InitSerialPort(void);
    void SearchAvailPort(void);

    Ui::MainWindow *ui;
    QSerialPort serial;
};

#endif // MAINWINDOW_H
