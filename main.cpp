#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("toy02.png"));
    splash->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splash->showMessage(QObject::tr("Setting up ..."),Qt::AlignCenter, Qt::green);
    MainWindow w;
    w.setWindowTitle("hao1-007gc");
    w.setFixedSize(1030,650);
    /*
        银河白    #FFFFFF   RGB(255, 255, 255)
        杏仁黄    #FAF9DE   RGB(250, 249, 222)
        秋叶褐    #FFF2E2   RGB(255, 242, 226)
        胭脂红    #FDE6E0   RGB(253, 230, 224)
        青草绿    #E3EDCD   RGB(227, 237, 205)
        海天蓝    #DCE2F1   RGB(220, 226, 241)
        葛巾紫    #E9EBFE   RGB(233, 235, 254)
        极光灰    #EAEAEF   RGB(234, 234, 239)
    */
    w.setStyleSheet(".QWidget{background:rgb(233, 235, 254);color:black;}");  //该处比下面多.QWidget，从而不影响子组件
    //w.setStyleSheet("background:rgb(110, 190, 10);color:black;");    //该颜色为亮绿色
    splash->showMessage(QObject::tr("Loading modules..."),Qt::AlignCenter, Qt::green);
    w.show();
    w.move ((QApplication::desktop()->width() - w.width())/2,(QApplication::desktop()->height() - w.height())/2);
    splash->finish(&w);
    delete splash;
    return app.exec();
}
