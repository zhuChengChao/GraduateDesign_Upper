#include "params.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"

using namespace cv;


//打开摄像头
void MainWindow::on_pushButton_OpenCam_clicked()
{
    if (!Capture.isOpened())
    {
        Capture.open(ui->comboBox_channel->currentText().toInt());

        Capture.set(CV_CAP_PROP_FRAME_WIDTH, CAPTURE_WIDTH);
        Capture.set(CV_CAP_PROP_FRAME_HEIGHT, CAPTURE_HEIGHT);

        if (Capture.isOpened())
        {
            //每次打开相机都初始化一次参数
            //InitRemap();
            timer->start(30);
            ui->pushButton_OpenCam->setText(tr("关闭相机"));
            ui->comboBox_channel->setDisabled(true);
        }
    }
    else
    {
        timer->stop();
        Capture.release();
        ui->pushButton_OpenCam->setText(tr("打开相机"));
        ui->comboBox_channel->setEnabled(true);
    }
}


//获取图像
void MainWindow::GetFrame()
{
    timer->stop();

    Capture>>ImageOriginal; //获取原图

    //修正
    //...

    cvtColor(ImageOriginal, ImageGray, CV_BGR2GRAY);
    blur(ImageGray, ImageGray, Size(BlurSize,BlurSize));
    //threshold(ImageGray, ImageBinary, Thresh, 255, THRESH_BINARY);
    //临时测试
    threshold(ImageGray, ImageBinary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    ShowImage();

    timer->start();
}


//显示图像
void MainWindow::ShowImage()
{
    QImage image;
    Mat showImage;

    //显示原图
    cv::resize(ImageOriginal,showImage,Size(ui->label_original->width(),ui->label_original->height()));
    cvtColor(showImage,showImage,CV_BGR2RGB);
    image = QImage((uchar*)showImage.data,showImage.cols,showImage.rows,
                   showImage.cols*showImage.channels(),QImage::Format_RGB888);
    ui->label_original->setPixmap(QPixmap::fromImage(image));
    ui->label_original->show();

    //灰度图
    cv::resize(ImageGray,showImage,Size(ui->label_original->width(),ui->label_original->height()));
    image = QImage((uchar*)showImage.data,showImage.cols,showImage.rows,
                   showImage.cols*showImage.channels(),QImage::Format_Grayscale8);
    ui->label_gray->setPixmap(QPixmap::fromImage(image));
    ui->label_gray->show();

    //二值化图
    cv::resize(ImageBinary,showImage,Size(ui->label_original->width(),ui->label_original->height()));
    image = QImage((uchar*)showImage.data,showImage.cols,showImage.rows,
                   showImage.cols*showImage.channels(),QImage::Format_Grayscale8);
    ui->label_binary->setPixmap(QPixmap::fromImage(image));
    ui->label_binary->show();
}
