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

//分析图像
void MainWindow::AnalysisImage(void)
{
    int findContoursNum = 0;
    findContoursNum = FindBinaryConters();

    //当轮廓数目大于3 寻找目标和车
    if(findContoursNum >= 3)
    {
        findCarAndPoint();
        GetAngle();
    }
    else
    {
        //当找到的轮廓数小于3个时的处理
        ;
    }

    SendData();
}


//寻找二值化图中的轮廓
//返回值：找到的轮廓数量
int MainWindow::FindBinaryConters(void)
{
    contours.clear();
    hierarchy.clear();
    ContourResults.clear();

    findContours(ImageBinary, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

    //获取较大面积的轮廓
    for(uint num = 0;num < contours.size();num++)
    {
        Contour tempContour;
        tempContour.m_Area = contourArea(contours[num]);
        if(tempContour.m_Area > MinBlobArea)
        {
            Moments mu = moments(contours[num],false);
            tempContour.m_Center = Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
            ContourResults.push_back(tempContour);
        }
    }

    return ContourResults.size();
}

//寻找车的目标物体
//计算目标与车的距离
void MainWindow::findCarAndPoint()
{
    // 按照轮廓面积由大到小排序
    for(uint i=0;i < ContourResults.size();i++)
    {
        for(uint j=0;j<ContourResults.size()-i-1;j++)
        {
            if(ContourResults[j].m_Area<ContourResults[j+1].m_Area)
            {
                Contour tmp = ContourResults[j];
                ContourResults[j] = ContourResults[j+1];
                ContourResults[j+1] = tmp;
            }
        }
    }

    float vec1to2_len,vec1to3_len,vec2to3_len;
    vec1to2_len = GetDistance(ContourResults[0].m_Center,ContourResults[1].m_Center);
    vec1to3_len = GetDistance(ContourResults[0].m_Center,ContourResults[2].m_Center);
    vec2to3_len = GetDistance(ContourResults[1].m_Center,ContourResults[2].m_Center);

    //最远的是信标灯，剩下面积最大的是车尾
    if((vec1to2_len<vec1to3_len)&&(vec1to2_len<vec2to3_len))
    {
        PtBeacon = ContourResults[2].m_Center;

        PtCarTail = ContourResults[0].m_Center;
        PtCarFront = ContourResults[1].m_Center;

        Distance = (short)vec2to3_len;
    }
    else if((vec1to3_len<vec1to2_len)&&(vec1to3_len<vec2to3_len))
    {
        PtBeacon = ContourResults[1].m_Center;

        PtCarTail = ContourResults[0].m_Center;
        PtCarFront = ContourResults[2].m_Center;

        Distance = (short)vec2to3_len;
    }
    else if((vec2to3_len<vec1to2_len)&&(vec2to3_len<vec1to3_len))
    {
        PtBeacon = ContourResults[0].m_Center;

        PtCarTail = ContourResults[1].m_Center;
        PtCarFront = ContourResults[2].m_Center;

        Distance = (short)vec1to3_len;
    }

    ui->lineEdit_Distance->setText(QString::number(Distance));
}


//得到两点距离
float MainWindow::GetDistance(Point2f &pt1,Point2f &pt2)
{
    float deltaX = pt1.x - pt2.x;
    float deltaY = pt1.y - pt2.y;

    //deltaY = deltaY*HV_RATIO;
    return sqrt(deltaX*deltaX + deltaY*deltaY);
}


//计算得到信标灯、车头、车尾构成三角形的角度
void MainWindow::GetAngle()
{
    float Len_Head2Tail,Len_Head2Beacon,Len_Tail2Beacon;

    Len_Head2Tail = GetDistance(PtCarTail,PtCarFront);
    Len_Tail2Beacon = GetDistance(PtCarTail,PtBeacon);
    Len_Head2Beacon = GetDistance(PtCarFront,PtBeacon);

    float cosval = (Len_Head2Tail*Len_Head2Tail+Len_Tail2Beacon*Len_Tail2Beacon-Len_Head2Beacon*Len_Head2Beacon)/
            (2*Len_Head2Tail*Len_Tail2Beacon);

    //车尾指向车头为a向量
    //车尾指向目标为b向量
    float vector_ax = PtCarFront.x - PtCarTail.x;
    float vector_ay = PtCarFront.y - PtCarTail.y;
    float vector_cx = PtBeacon.x - PtCarTail.x;
    float vector_cy = PtBeacon.y - PtCarTail.y;
    float dir = (vector_ax * vector_cy) - (vector_ay - vector_cx);

    DirAngle = (dir>0)?0:1;

    Angle = acos(cosval)*180/CV_PI;

    QString showDir = (DirAngle > 0)? "right" : "left";
    ui->lineEdit_AngleDir->setText(showDir);
    ui->lineEdit_Angle->setText(QString::number(Angle));
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
