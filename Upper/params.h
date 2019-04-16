#ifndef PARAMS_H
#define PARAMS_H

/*
 * 说明：程序中的一些参数宏定义
 */
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <opencv2/opencv.hpp>

#define CAPTURE_WIDTH 640
#define CAPTURE_HEIGHT 480

//0：帧头0xa0  1：角度方向  2：角度大小    3:距离低8位
//4:距离高8位   5:保留位   6：sum校验位   7：帧尾0x0a
extern uchar TxBuff[8];

extern bool g_isMovie;

//定义Counter类
class Contour
{
public:
    int index;
    float m_Area;
    cv::Point2f m_Center;
};


#endif // PARAMS_H
