#ifndef MYSPEED_H
#define MYSPEED_H

#include <QWidget>
#include <QTimer>
#include<math.h>
extern float nowRoll,nowPitch,nowYaw;
class mySpeed : public QWidget
{
    Q_OBJECT
    
public:
    mySpeed(QWidget *parent = 0);
    ~mySpeed();

protected:
    void paintEvent(QPaintEvent *);

    void drawCrown(QPainter *painter);                                                      //画表冠，渐变色
    void drawBackground(QPainter *painter,float roll,float pitch);          //画内部俯仰盘，横滚盘

    void drawRollNumericValue(QPainter *painter,float roll);                  //画横滚刻度（需要转动）
    void drawYawShow(QPainter *painter,float yaw);                                 //画偏航刻度（需要转动）
    void drawPitchNumericValue(QPainter *painter,float roll,float pitch);  //画俯仰刻度盘跟着动

    void drawTextPie(QPainter *painter,float yaw,float roll,float pitch,float height);//画固定的形状，填姿态值

private:
    float att_height;
    QTimer *m_updateTimer;


public Q_SLOTS:
      void UpdateAngle();
};

#endif // MYSPEED_H
