#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "params.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

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
    //图像
    void GetFrame(void);
    void on_pushButton_OpenCam_clicked();

    //参数修改
    void on_spinBox_ThresValue_valueChanged(int arg1);
    void on_spinBox_BlurSize_valueChanged(int arg1);
    void on_spinBox_MinBlibSize_valueChanged(int arg1);

private:
    //函数定义：
    //初始化函数
    void AllIint(void);
    //串口相关函数：
    void InitSerialPort(void);
    void SearchAvailPort(void);
    //图像
    void ShowImage(void);
    void AnalysisImage(void);
    int FindBinaryConters(void);
    //计算函数
    void findCarAndPoint(void);
    float GetDistance(Point2f &pt1, Point2f &pt2);
    void GetAngle();

    Ui::MainWindow *ui;
    QSerialPort serial;
    //摄像头图像
    QTimer *timer;
    VideoCapture Capture;
    Mat ImageOriginal,ImageGray,ImageBinary;
    //调节参数
    int BlurSize;
    int Thresh;
    int MinBlobArea;

    //检测到的轮廓
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Contour> ContourResults;

    //车灯的位置信息
    Point2f PtBeacon, PtCarFront, PtCarTail;
    unsigned int Distance;
    unsigned char DirAngle,Angle;
};

#endif // MAINWINDOW_H
