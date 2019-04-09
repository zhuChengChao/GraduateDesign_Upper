#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "params.h"
#include "opencv2/opencv.hpp"

using namespace cv;

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
    //测试按钮
    void on_pushButton_clicked();
    //图像
    void GetFrame(void);
    void on_pushButton_OpenCam_clicked();

private:
    //函数定义：
    //初始化函数
    void AllIint(void);
    //串口相关函数：
    void InitSerialPort(void);
    void SearchAvailPort(void);
    //图像
    void ShowImage(void);

    Ui::MainWindow *ui;
    QSerialPort serial;
    //摄像头图像
    QTimer *timer;
    VideoCapture Capture;
    Mat ImageOriginal,ImageGray,ImageBinary;
    int BlurSize;
    int Thresh;
};

#endif // MAINWINDOW_H
