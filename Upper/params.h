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

#define CAPTURE_WIDTH 480
#define CAPTURE_HEIGHT 360

//0：帧头0xa0  1：控制对象 0x11车  0x22摄像头  0x33机械臂  2：控制方式  3-5:保留  6：sum  7：帧尾0x0a
extern uchar TxBuff[8];


#endif // PARAMS_H
