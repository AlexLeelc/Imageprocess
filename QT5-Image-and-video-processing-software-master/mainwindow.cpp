#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QApplication>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>
#include <string>
#include <QVector>
using namespace std;
#include <iostream>
#include <sstream>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QImage>
#include <QtCore/qmath.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/imgproc/imgproc_c.h"///for cvSmooth

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);          

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

}

MainWindow::~MainWindow()
{
    delete ui;
}

void split(const string& s,vector<int>& sv,const char flag = ' ') {
    sv.clear();
    istringstream iss(s);
    string temp;

    while (getline(iss, temp, flag)) {
        sv.push_back(stoi(temp));
    }
    return;
}



//图片居中显示,图片大小与label大小相适应
QImage MainWindow::ImageCenter(QImage  qimage,QLabel *qLabel)
{
    QImage image;
    QImage image_show = qimage;
    QSize imageSize = image_show.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
            if (dWidthRatio>dHeightRatio)
            {
                image = image_show.scaledToWidth(labelSize.width());
            }
            else
            {
                image = image_show.scaledToHeight(labelSize.height());
            }
            return image;

}

//虚拟rgb值
void MainWindow::virtual_rgb(QImage image)
{
    int pixels = image.width() * image.height();
    value_light_last=0;
    unsigned int *data = (unsigned int *)image.bits();
    for (int i = 0; i < pixels; ++i)
    {

        if(redt.size()<pixels)
        {
            redt.append(qRed(data[i]));
            greent.append(qGreen(data[i]));
            bluet.append(qBlue(data[i]));
        }
        else
        {
            redt[i]=qRed(data[i]);
            greent[i]=qGreen(data[i]);
            bluet[i]=qBlue(data[i]);
        }
    }
}

//界面上选择图片
void MainWindow::on_pushButton_clicked()
{
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "E:/Qt/qtworks/MainWindow/images", tr("图像文件(*.jpg *.png *.bmp)"));
    if(srcDirPathListS.size()==1){
        value_light_last=0;
        srcDirPathList =srcDirPathListS;
        srcDirPathListS.clear();
        index =0;
        QString srcDirPath = srcDirPathList.at(index);//寻找第0个存储路径
        //QImage image(srcDirPath);
        origin_path=srcDirPath;
        QImage_show = QImage(origin_path);
        QImage Image=ImageCenter(QImage_show,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        QImage images=ImageCenter(QImage_show,ui->label_other);
        ui->label_other->setPixmap(QPixmap::fromImage(images));
        ui->label_other->setAlignment(Qt::AlignCenter);
        ui->label_status->setAlignment(Qt::AlignRight);
        ui->label_status->setText("导入成功");
        ui->horizontalSlider_gamma->setValue(100);
        ui->label_gamma->setText(QString::number(1));
        /*QVector<int>().swap(redt);//导入新图要清空虚拟rgb
        QVector<int>().swap(greent);
        QVector<int>().swap(bluet);*/
        virtual_rgb(QImage_show);
     }
}

//对比
void MainWindow::on_pushButton_contrast_clicked()
{
    QImage Image=ImageCenter(QImage_show,ui->label_other);
    ui->label_other->setPixmap(QPixmap::fromImage(Image));
    ui->label_status->setText("当前图片已导入候选框以作对比");
}

//复原按钮
void MainWindow::on_pushButton_origin_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage_show = image;
        virtual_rgb(QImage_show);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        ui->horizontalSlider->setValue(0);
        ui->label_status->setText("图像已复原");
    }else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//界面中的保存
void MainWindow::on_pushButton_save_clicked()
{
    //QImage image2(origin_path);
    QImage image2=QImage_show;
    if(ui->label_show->pixmap()!=nullptr){
        QString filename = QFileDialog::getSaveFileName(this,
        tr("保存图片"),
        "E:/Qtworks/MainWindow/images",
        tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
        if (filename.isEmpty())
        {
            return;
        }
        else
        {
            if (!(image2.save(filename))) //保存图像
            {

                QMessageBox qmess;
                qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
                qmess.information(this,
                tr("图片保存成功！"),
                tr("图片保存失败！"));
                ui->label_status->setText("保存成功");
                return;
            }
        }
    }
    else
    {
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }

}

//加权平均值灰度化
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);//rgb+alpha透明通道
    QColor oldColor;
    for(int y = 0; y < newImage.height(); y++)
    {
        for(int x = 0; x < newImage.width(); x++)
        {
            oldColor = QColor(image.pixel(x,y));
            int average = oldColor.red()*0.3 + oldColor.green()*0.59 + oldColor.blue()*0.11;
            newImage.setPixel(x, y, qRgb(average, average, average));
        }
    }
    return newImage;
}

//灰度化
void MainWindow::on_pushButton_gray_clicked()
{

    if(origin_path!=nullptr){
    //QImage image(origin_path);

    QImage_show=gray(QImage_show);
    virtual_rgb(QImage_show);
    QImage Image=ImageCenter(QImage_show,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));//QLabel控件中我们可以使用setPixmap函数显示图片
    ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//均值滤波
QImage MainWindow::junzhi(QImage image){
    int sizeKernel = 3;
    int sumKernel = 9;//将像素点周围九个格子的rgb值取平均值放入该点
    QColor color;
    for(int x = sizeKernel/2;x<image.width() - sizeKernel/2;x++)//定点，边框位置不能取
    {
       for(int y= sizeKernel/2;y<image.height() - sizeKernel/2;y++)
       {
           int r = 0;
           int g = 0;
           int b = 0;
           for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)//循环找点周围的8个格子
           {
               for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
               {
                    color = QColor(image.pixel(x+i,y+j));
                    r+=color.red();
                    g+=color.green();
                    b+=color.blue();
               }
           }
           r = qBound(0,r/sumKernel,255);//将r/sumKernel传给了r，范围设置在0-255
           g = qBound(0,g/sumKernel,255);
           b = qBound(0,b/sumKernel,255);
           image.setPixel(x,y,qRgb( r,g,b));
        }
     }
     return image;
}


//均值滤波
void MainWindow::on_pushButton_junzhi_clicked()
{
    if(origin_path!=nullptr){
    //QImage image(origin_path);
        QImage_show =junzhi(QImage_show);
        virtual_rgb(QImage_show);
             QImage Image=ImageCenter(QImage_show,ui->label_show);
             ui->label_show->setPixmap(QPixmap::fromImage(Image));
             ui->label_show->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//中值滤波
int MainWindow::AveColor (int *a){
    for (int i  = 0; i < 9; i++){
        for (int j = 0; j < 8;j++){
            if (a[j] > a[j+1]){
                int temp = a[j+1];
                a[j+1] = a[j];
                a[j] = temp;
            }
        }
    }
    return a[4];
}

//中值滤波
QImage MainWindow::zhongzhi(QImage image){
    int sizeKernel = 3;
    int r[9];
    int g[9];
    int b[9];
    for (int i = 0; i < 9; i++){
        r[i] = g[i] = b[i] = 0;
    }
    int m = 0,n = 0,p = 0;
    QColor color;
    for(int x = sizeKernel/2;x<image.width() - sizeKernel/2;x++)//定点，边框位置不能取
    {
        for(int y= sizeKernel/2;y<image.height() - sizeKernel/2;y++)
        {
            int rt = 0;
            int gt = 0;
            int bt = 0;
            for(int i = -sizeKernel/2;i<=sizeKernel/2;i++)//循环找点周围的8个格子
            {
                for(int j = -sizeKernel/2;j<=sizeKernel/2;j++)
                {
                    color = QColor(image.pixel(x+i,y+j));
                    r[m++] = color.red();
                    g[n++] = color.green();
                    b[p++] = color.blue();
                }
            }
            m = n = p = 0;
            rt = qBound(0,AveColor(r),255);
            gt = qBound(0,AveColor(g),255);
            bt = qBound(0,AveColor(b),255);
            image.setPixel(x,y,qRgb( rt,gt,bt));
        }
    }
    return image;
}

void MainWindow::on_pushButton_zhongzhi_clicked()
{
    if(origin_path!=nullptr){
    //QImage image(origin_path);
        QImage_show =zhongzhi(QImage_show);
        virtual_rgb(QImage_show);
             QImage Image=ImageCenter(QImage_show,ui->label_show);
             ui->label_show->setPixmap(QPixmap::fromImage(Image));
             ui->label_show->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//亮度调节
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(origin_path!=nullptr){
    QImage image = QImage_show;
    int red, green, blue;
        int pixels = image.width() * image.height();
        unsigned int *data = (unsigned int *)image.bits();
        for (int i = 0; i < pixels; ++i)
        {

            redt[i]= redt[i]+ value-value_light_last;//虚拟red值
            red = (redt[i] < 0x00) ? 0x00 : (redt[i] > 0xff) ? 0xff : redt[i];//实际的red值
            greent[i]= greent[i]+ value-value_light_last;//虚拟green值
            green = (greent[i] < 0x00) ? 0x00 : (greent[i] > 0xff) ? 0xff : greent[i];//实际的green值
            bluet[i]= bluet[i]+ value-value_light_last;//虚拟blue值
            blue = (bluet[i] < 0x00) ? 0x00 : (bluet[i] > 0xff) ? 0xff : bluet[i];//实际的blue值
            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }
        QImage_show = image;
        QImage Image=ImageCenter(QImage_show,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        ui->label_light->setText(QString::number(value));
        value_light_last=value;
    }
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }

}

//Otsu算法
void MainWindow::cal_p(QImage image)//灰度级k的出现概率
{
    for(int k=0;k<=255;k++){
        int count=0;
        for(int y = 0; y < image.height(); y++)
        {
         for(int x = 0; x < image.width(); x++)
            {
                QColor color = QColor(image.pixel(x,y));
                if(color.red()==k){
                    count++;
                }
            }
        }
        p.append(double(count)/double((image.height()*image.width())));
    }
}

QImage MainWindow::Otsu(QImage image)//求阈值
{

    image = gray(image);
    cal_p(image);
    double u;
    double T=0;
    int te=0;
    double max=0;
    for(int t =0;t<=255;t++)
    {
        double wb=0;
        double wo=0;
        double ub=0;
        double uo=0;
        for(int k=0;k<=t;k++)
        {
            wb=wb+p[k];
        }
        for(int k=t+1;k<=255;k++)
        {
            wo=wo+p[k];
        }
        //qDebug()<<wo;
        for(int k=0;k<=t;k++)
        {
            if(wb==0)
                ub+=0;
            else
                ub=ub+double(k)*p[k]/wb;
        }
        for(int k=t+1;k<=255;k++)
        {
            if(wo==0)
                uo+=0;
            else
                uo=uo+double(k)*p[k]/wo;
        }
        u=wb*ub+wo*uo;
        T=wb*(ub-u)*(ub-u)+wo*(uo-u)*(uo-u);
        if(T>max)
        {
            te=t;
            max=T;
        }
    }
    ui->label_status->setText("当前阈值设为："+QString::number(te));

    int height=image.height();
    int width=image.width();
            int bt;
            QColor oldColor;
            for (int i = 0; i < height; ++i)
            {
                for(int j=0;j<width;++j){
                    oldColor = QColor(image.pixel(j,i));
                    bt = oldColor.red();
                if(bt<te){
                    bt=0;
                }else{
                    bt=255;
                }
                image.setPixel(j,i, qRgb(bt, bt, bt));

                }
            }
    QVector<double>().swap(p);
    return image;
}


void MainWindow::on_pushButton_Otsu_clicked()
{
    if(origin_path!=nullptr){
    QImage_show =Otsu(QImage_show);
    virtual_rgb(QImage_show);
    QImage Image=ImageCenter(QImage_show,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//边缘检测
QImage MainWindow::bianyuan(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a=0,a1=0,a2=0,a3 = 0;
    for( int y = 0; y < image.height() - 1; y++)
    {
        for(int x = 0; x < image.width() - 1; x++)
        {
            color0 =   QColor ( image.pixel(x,y));
            color1 =   QColor ( image.pixel(x + 1,y));
            color2 =   QColor ( image.pixel(x,y + 1));
            color3 =   QColor ( image.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            newImage.setPixel(x,y,qRgb(a,a,a));
        }
    }
    return newImage;
}



//边缘检测
void MainWindow::on_pushButton_junzhi_2_clicked()
{
    if(origin_path!=nullptr){
    //QImage image(origin_path);
    //QImage temp =gray(image);
    QImage_show =bianyuan(QImage_show);
    virtual_rgb(QImage_show);
    QImage Image=ImageCenter(QImage_show,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//伽马变换
QImage MainWindow::gamma(QImage image,int value){
    double d=double(value)/double(100);
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

void MainWindow::on_pushButton_gamma_cancel_clicked()
{
    ui->horizontalSlider_gamma->setValue(100);
    ui->label_gamma->setText(QString::number(1));
}

void MainWindow::on_pushButton_gamma_confirm_clicked()
{
    QImage_show=gamma(QImage_show,gamma_value);
    virtual_rgb(QImage_show);
    ui->horizontalSlider_gamma->setValue(100);
    ui->label_gamma->setText(QString::number(1));
}

void MainWindow::on_horizontalSlider_gamma_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image=gamma(QImage_show,value);
        gamma_value=value;
        ui->label_gamma->setText(QString::number(double(value)/double(100)));
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//光栅扫描追踪
QImage MainWindow::Edge_tracking(QImage image,int value1,int value2)
{
    QImage newImage =gray(image).convertToFormat(QImage::Format_ARGB32);
    QImage newImage2 =gray(image).convertToFormat(QImage::Format_ARGB32);
    for( int y = 0; y < newImage2.height() - 1; y++)
    {
        for(int x = 0; x < newImage2.width() - 1; x++)
        {
            if(QColor(newImage2.pixel(x,y)).red()>=value1)
            {
                newImage.setPixel(x,y,qRgb( 0,0,0));
                int temp = QColor(newImage2.pixel(x,y)).red();
                if(y<newImage2.height() - 2)
                {
                    if(x>0&&x<newImage2.width() - 2)
                    {
                        for(int k =-1;k<2;k++)
                        {
                            if(abs(temp-QColor(newImage2.pixel(x+k,y)).red())<=value2)
                                newImage.setPixel(x+k,y,qRgb( 0,0,0));
                            else
                                newImage.setPixel(x+k,y,qRgb( 255,255,255));
                        }
                    }
                }

            }
            else
                newImage.setPixel(x,y,qRgb( 255,255,255));
        }
    }
    return newImage;
}

void MainWindow::on_pushButton_track_cancel_clicked()
{
    ui->horizontalSlider_track_1->setValue(128);
    ui->horizontalSlider_track_2->setValue(128);
    ui->label_track_2_num->setText(QString::number(128));
    ui->label_track_3_num->setText(QString::number(128));
}

void MainWindow::on_pushButton_track_confirm_clicked()
{
    QImage_show=Edge_tracking(QImage_show,ui->horizontalSlider_track_1->value(),ui->horizontalSlider_track_2->value());
    virtual_rgb(QImage_show);
}

void MainWindow::on_horizontalSlider_track_1_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image=Edge_tracking(QImage_show,value,ui->horizontalSlider_track_2->value());
        ui->label_track_2_num->setText(QString::number(value));
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

void MainWindow::on_horizontalSlider_track_2_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image=Edge_tracking(QImage_show,ui->horizontalSlider_track_1->value(),value);
        ui->label_track_3_num->setText(QString::number(value));
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox qmess;
        qmess.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        qmess.warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}
