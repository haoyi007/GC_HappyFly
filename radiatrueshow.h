#ifndef RADIATRUESHOW_H
#define RADIATRUESHOW_H

#include "global.h"
#include <QDialog>
#include <QPointF>


namespace Ui {
class radiaTrueShow;
}

class radiaTrueShow : public QDialog
{
    Q_OBJECT

public:
    explicit radiaTrueShow(QWidget *parent = 0);
    ~radiaTrueShow();
    QTimer *radiaTimer;
    QPixmap *newPix ;
    RPlidar_Data_Typedef  RPlidar_Body_Data[256];//类成员不可以在定义时初始化
    int receiveNum[100];
    void doRadar();
    void paintEvent(QPaintEvent *event);
    char data_to_send[50];	//发送数据缓存
    float draw_roll,draw_pitch,draw_yaw,draw_current_Yaw;
    float now_yaw_here,now_roll_here,now_pitch_here,now_taretyaw_here;

public Q_SLOTS:
        void receiveURGDataSlot2(RPlidar_Data_Typedef arr[],int whitch);
        void receiveATISlot2(float roll,float pitch,float yaw,float current_Yaw);
        void receiveNowAttiSlot2(float roll,float pitch,float yaw,float target_Yaw);
        void connectURGData();
Q_SIGNALS:
        void senddata(char *TxData,int num);

private:
    Ui::radiaTrueShow *ui;
    QPointF *pointf[260];    // 画多个点
    QPixmap *pix;
};

#endif // RADIATRUESHOW_H
