#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QTimer>
#include <QImage>
#include <QMessageBox>
#include <QVector>
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
    void showascii();
    int new_image_in;//记录新图是否导入
    QImage gray(QImage image);//灰度化
    QImage gray2(QImage image);//灰度化2
    QImage AdjustContrast(QImage image, int value);//调整对比度
    QImage ImageCenter(QImage  qimage,QLabel *qLabel);//调整图片比例
    QImage Otsu(QImage image);//Otsu
    QVector<double>p;//记录灰度级k在一张图中出现的概率以减少时间复杂度
    void cal_p(QImage image);//灰度级k的出现概率
    QImage bianyuan(QImage image);//边缘
    QStringList srcDirPathList;//图片list
    int index =0;//图片index
    int value_light_last;//记录亮度滑动条上一个值
    QVector<int>redt;//调节亮度存储虚拟r通道（考虑溢出）
    QVector<int>greent;//调节亮度存储虚拟g通道（考虑溢出）
    QVector<int>bluet;//调节亮度存储虚拟b通道（考虑溢出）
    void virtual_rgb(QImage image);
    int cal_value_light_last(int value);
    QString stom(int s);
    QImage junzhi(QImage image);
    QImage zhongzhi(QImage image);
    int AveColor (int *a);
    QImage fuhe(QImage images);
    QImage gamma(QImage image,int value);
    int gamma_value;
    QImage Edge_tracking(QImage image,int value1,int value2);
    QImage QImage_show;//新加入目的使得图像处理是对QImage_show进行的，而不是针对label中的图像

private slots:

    void on_pushButton_clicked();

    void on_pushButton_gray_clicked();

    void on_pushButton_junzhi_clicked();

    void on_pushButton_zhongzhi_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_junzhi_2_clicked();

    void on_pushButton_Otsu_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_origin_clicked();

    void on_pushButton_contrast_clicked();

    void on_horizontalSlider_gamma_valueChanged(int value);

    void on_pushButton_gamma_confirm_clicked();

    void on_pushButton_gamma_cancel_clicked();

    void on_pushButton_track_cancel_clicked();

    void on_pushButton_track_confirm_clicked();

    void on_horizontalSlider_track_1_valueChanged(int value);

    void on_horizontalSlider_track_2_valueChanged(int value);



private:
    Ui::MainWindow *ui;
    bool language=true;
    bool isstart=false;
    QString origin_path;//目前处理的图片的原图
    QMessageBox customMsgBox;

};

#endif // MAINWINDOW_H
