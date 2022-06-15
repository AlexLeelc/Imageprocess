#include "unistd.h"
#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QLabel>
#include <QMovie>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/myImage/images/loading.gif");
        QSplashScreen splash(pixmap);
        QLabel label(&splash);
        splash.show();
        splash.setCursor(Qt::BlankCursor);
        MainWindow w;
        w.setWindowTitle(QObject::tr("Lc & Wmx的图像处理软件"));
        w.setWindowIcon(QIcon("D:/lc/QT5-Image-and-video-processing-software-master/images/logo.png"));
        w.show();
        splash.finish(&w);
    return a.exec();
}
