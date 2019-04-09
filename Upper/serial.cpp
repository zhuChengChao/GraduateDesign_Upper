/*
 * 说明：串口通讯
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "params.h"

//初始化串口
void MainWindow::InitSerialPort(void)
{
    serial.setPortName(ui->comboBox_com->currentText());
    serial.setBaudRate(ui->comboBox_baud->currentText().toInt());
    serial.setDataBits(QSerialPort::Data8);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
}


//查询可用串口资源
void MainWindow::SearchAvailPort()
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);

        ui->comboBox_com->addItem(serial.portName());
    }
}


//查询可用的串口资源
void MainWindow::on_pushButton_searchPort_clicked()
{
    if(!serial.isOpen())
        SearchAvailPort();
}


//开串口槽函数
void MainWindow::on_pushButton_openPort_clicked()
{
    if(!serial.isOpen())
    {
        InitSerialPort();
        serial.close();

        if(serial.open(QIODevice::ReadWrite))
        {
            connect(&serial,SIGNAL(readyRead()),this,SLOT(ReceiveData()));
            ui->pushButton_openPort->setText(tr("关闭串口"));
            ui->comboBox_com->setDisabled(true);
            ui->comboBox_baud->setDisabled(true);
        }
    }
    else
    {
        serial.close();
        ui->pushButton_openPort->setText(tr("打开串口"));
        ui->comboBox_baud->setDisabled(false);
        ui->comboBox_com->setDisabled(false);
    }
}


//接受函数：此处不需要接收
void MainWindow::ReceiveData()
{
    qDebug()<<"ReceiveData is None";
}


//数据发送
void MainWindow::SendData()
{
    ushort sum = 0;
    //memset(TxBuff, 10, 8);
    TxBuff[0] = 0xa0;
    TxBuff[1] = DirAngle;
    TxBuff[2] = Angle;
    TxBuff[3] = Distance & 0x00ff;
    TxBuff[4] = Distance / 256;
    TxBuff[5] = 0x00;

    for(uint i=1;i<6;i++)
        sum += TxBuff[i];
    TxBuff[6] = sum & 0x00ff;
    TxBuff[7] = 0x0a;


    if(serial.isOpen())
        serial.write((const char*)TxBuff,8);
}

