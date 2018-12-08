#include "myspeed.h"
#include <QPainter>
#include <qmath.h>
#include<QDebug>
mySpeed::mySpeed(QWidget *parent)
    : QWidget(parent)
{
    att_height=0;
    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(1000);
    connect(m_updateTimer,SIGNAL(timeout()),this,SLOT(UpdateAngle()));
    m_updateTimer->start();//启动定时器
    setAttribute(Qt::WA_Disabled);
    resize(400,400);         //窗体大小
}

mySpeed::~mySpeed()
{

}

void mySpeed::drawCrown(QPainter *painter)  //绘制表冠，绘制顺序1
{
    painter->save();                                                   /* 之前已经将坐标变换为窗体中心 */
    int radius =100;
    QLinearGradient lg1(0,-radius,0,radius);         //线性渐变(QLinearGradient)，至顶而底渐变
    lg1.setColorAt(0,Qt::darkBlue);                             //设置渐变的颜色和路径比例,(最上面为蓝色)
    lg1.setColorAt(1,Qt::darkGreen);                                 //（最下面为绿色）
    painter->setBrush(lg1);                                      // 创建QBrush对象,把这个渐变对象传递进去：
    painter->setPen(Qt::green);
    painter->drawEllipse(-100, -100, 200 ,200);    //该内环内全部填充渐变色
    painter->restore();
}

void mySpeed::drawYawShow(QPainter *painter,float yaw)   //绘制磁航向刻度，需要根据yaw进行转动
{
    //yaw参数输入范围0-360度，0度对应正北方向
    painter->save();
    painter->rotate(-yaw);

    QFont font1("Microsoft YaHei",7,75);  //字体、大小、加粗权重
    painter->setFont(font1);
    QFontMetricsF fm(this->font());          //能得到字符串的像素数
    double w1, w2;
    QString E="E",NE="NE",N="N",NW="NW",W="W",SW="SW",S="S",SE="SE";
    w1 = fm.size(Qt::TextSingleLine,E).width();
    w2 = fm.size(Qt::TextSingleLine,NE).width();

    painter->setPen(Qt::white);
    QPen pen = painter->pen();
    double angleStep = 180.0/ 40;                                //每一个份数的角度
    for (int i = 0; i <= 80; i++)
    {
        if (i % 10 == 0)                                                      //整数刻度显示加粗
        {
            pen.setWidth(1);                                              //设置线宽
            painter->setPen(pen);
            painter->drawLine(0, 88, 0, 96);                    //长线
            if(i==0){
                            painter->drawText(-0.75*w1, -78,N);
            }
            if(i==10){
                            painter->drawText(-0.75*w2,-78 ,NW);
            }
            if(i==20){
                            painter->drawText(-0.75*w1, -78,W);
            }
            if(i==30){
                            painter->drawText(-0.75*w2,-78 ,SW);
            }
            if(i==40){
                            painter->drawText(-0.75*w1, -78,S);
            }
            if(i==50){
                            painter->drawText(-0.75*w2,-78 ,SE);
            }
            if(i==60){
                            painter->drawText(-0.75*w1, -78,E);
            }
            if(i==70){
                            painter->drawText(-0.75*w2,-78 ,NE);
            }
        }
        else
        {
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawLine(0, 88, 0, 91);                          //短线
        }
        painter->rotate(-angleStep);                                     //旋转每一份数的角度
    }

    painter->restore();
}

void mySpeed::drawRollNumericValue(QPainter *painter,float roll)   //roll表盘转动
{
    //左负右正
    painter->save();
    painter->rotate(180);
    QLinearGradient lg1(0,100,0,-100);                        //线性渐变(QLinearGradient)，至顶而底渐变
    lg1.setColorAt(0,Qt::darkBlue);
    lg1.setColorAt(1,Qt::darkGreen);
    painter->setBrush(lg1);                                      // 创建QBrush对象,把这个渐变对象传递进去
    const QRect rectangle = QRect(-100, -100, 200 ,200);
    painter->drawPie(rectangle, 0,16*180 );
    painter->restore();

    painter->save();
    painter->setPen(Qt::white);
    QFont font1("Microsoft YaHei",7,75);  //字体、大小、加粗权重
    painter->setFont(font1);
    QFontMetricsF fm(this->font());   //能得到字符串的像素数
    double w1,tmpVal;
    QString str;
    painter->rotate(-90+roll);                 //坐标系先逆时针旋转90度，y轴指向右,然后顺时针旋转roll度
    //注意旋转前绘制的东西，旋转后位置不动，只是坐标系的旋转
    QPen pen = painter->pen();
    int temp_180=0;
    double angleStep = 180.0/ 40;          //每一个份数的角度
    for (float i = 0; i <= 40; i++)
    {
        if((temp_180==1)&&(4.5*i+roll>=0)&&(4.5*i+roll<=180)){painter->rotate(180);temp_180=0;}
        if((temp_180==0)&&((4.5*i+roll>180)||(4.5*i+roll<0))){painter->rotate(180);temp_180=1;}
        if (qRound(i) % 10 == 0)                                                      //整数刻度显示加粗
        {
            pen.setWidth(1);                                              //设置线宽
            painter->setPen(pen);
            painter->drawLine(0, 80, 0, 86);                    //两个参数应该是两个坐标值，长线
            tmpVal=90-45*((int)i/10);
            if((temp_180==1)&&((4.5*i+roll>180)))tmpVal=tmpVal+180;
            if((temp_180==1)&&((4.5*i+roll<0)))tmpVal=tmpVal-180;
            str = QString( "%1" ).arg(tmpVal);
            w1 = fm.size(Qt::TextSingleLine,str).width();
            painter->drawText(-0.5*w1, 96,str);
        }
        else
        {
            pen.setWidth(0);
            painter->setPen(pen);
            painter->drawLine(0, 83, 0, 86);                          //短线
        }
        painter->rotate(angleStep);                                     //坐标系顺时针旋转每一份数的角度
    }
    painter->restore();
}

//QBrush brush(QColor(0,255,0,125));            //rgb,(red,green,blue),最后的125是透明度（alpha）设置
//painter->setBrush(brush);
void mySpeed::drawBackground(QPainter *painter,float roll,float pitch)  //绘制黑色内环，绘制顺序2（这里传入pitch参数）
{
    painter->save();                         //内环圆的半径是75
    painter->rotate(-roll);

    painter->setBrush( Qt::blue);
    const QRect rectangle = QRect(-75, -75,150,150);
    painter->drawChord(rectangle, 16*180/3.1415*asin(3.5*pitch/75), 16*(180-2*180/3.1415*asin(3.5*pitch/75)));          //上半圆

    painter->setBrush( Qt::darkGreen);
    const QRect rectangle2 = QRect(-75, -75,150,150);
    painter->drawChord(rectangle2, 16*(-180-180/3.1415*asin(3.5*pitch/75)), 16*(180+2*180/3.1415*asin(3.5*pitch/75)));          //下半圆
    painter->restore();
}

void mySpeed::drawPitchNumericValue(QPainter *painter,float roll,float pitch)    /*绘制中间pitch的刻度尺*/
{
        painter->save();
        painter->rotate(-roll);
        QPen pen ;
        pen.setColor(Qt::white);
        double tmpVal,h;
        QString str;
        QFont font1 = painter->font();
        font1.setPixelSize(6);
        painter->setFont(font1);
        QFontMetricsF fm(font());
        painter->translate(0,3.5*pitch);
        for (float i = -20-pitch; i <= 20-pitch; i++)
        {
            int i_temp=qRound(i);
            if (i_temp % 5 == 0)                                                      //整数刻度显示加粗
            {
                pen.setWidth(1);                                                                    //设置线宽
                painter->setPen(pen);
                painter->drawLine(-3, 3.5*i, 3,3.5*i);                                //长线
                tmpVal=-i_temp;
                str = QString( "%1" ).arg(tmpVal);
                h = fm.size(Qt::TextSingleLine,str).height();
                painter->drawText(10,3.5*i+0.15*h, str);       //字号需要缩小
            }
            else
            {
                pen.setWidth(0);
                painter->setPen(pen);
                painter->drawLine(-1.5, 3.5*i, 1.5, 3.5*i);                          //短线
            }
        }
        painter->restore();
}

void mySpeed::drawTextPie(QPainter *painter,float yaw,float roll,float pitch,float height)             //绘制固定的图饼标识
{
    QString string_Yaw,string_Roll,string_Pitch,string_Height;
    string_Yaw = QString( "%1" ).arg(yaw);
    string_Roll = QString( "%1" ).arg(roll);
    string_Pitch = QString( "%1" ).arg(pitch);
    string_Height = QString( "%1" ).arg(height);

    painter->save();
    painter->setPen(Qt::green);
    painter->setBrush( Qt::gray);

    const QRect rectangle = QRect(-132, -40, 60, 80);   //左半边
    painter->drawPie(rectangle, -16*74, 16*148);

    const QRect rectangle2 = QRect(72, -40, 60, 80);    //右半边
    painter->drawPie(rectangle2, 16*106, 16*148);

    painter->setPen(Qt::black);
    QFont font1("Microsoft YaHei",4,75);  //字体、大小、加粗权重
    painter->setFont(font1);
    painter->setBrush( Qt::darkGray);

    QString str_Yaw("YAW");
    painter->drawText(-93, -17, str_Yaw);
    painter->drawRect(-97,-15,20,10);

    QString str_Hig("HIG");
    painter->drawText(-93, 22, str_Hig);
    painter->drawRect(-97,5,20,10);

    QString str_Pitch("PIT");                       //该处字体不是很好
    painter->drawText(83, -17, str_Pitch);
    painter->drawRect(77,-15,20,10);

    QString str_Roll("ROL");
    painter->drawText(83, 22, str_Roll);
    painter->drawRect(77,5,20,10);

    QFont font2("Microsoft YaHei",5,75);  //字体、大小、加粗权重
    painter->setFont(font2);

    painter->setPen(Qt::white);                       //绘制具体数值
    painter->drawText(-92,-7,string_Yaw);
    painter->setPen(Qt::yellow);
    painter->drawText(82,13,string_Roll);
    painter->setPen(Qt::red);
    painter->drawText(82,-7,string_Pitch);
    painter->setPen(Qt::green);
    painter->drawText(-92,13,string_Height);

    QPen pen = painter->pen();
    painter->setBrush( Qt::white);
    const QRect rectangle_up = QRect(-15, -100, 30, 20);     //上三角
    painter->drawPie(rectangle_up, 16*75, 16*30);
    const QRect rectangle_down = QRect(-15, 75, 30, 20);   //下三角
    painter->setBrush( Qt::yellow);
    painter->drawPie(rectangle_down, 16*75, 16*30);
    pen.setColor(QColor(255,0,0,100));                                 //画竖垂线
    pen.setWidth(0.1);
    painter->setPen(pen);
    painter->drawLine(0,0,0,75);

    painter->rotate(-roll);
    pen.setColor(QColor(255,0,0,255));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(-55,0,-40,0);
    painter->drawLine(-15,6,0,0);
    painter->drawLine(0,0,15,6);
    painter->drawLine(40,0,55,0);
    painter->restore();
}

void mySpeed::paintEvent(QPaintEvent *)
{
    QPainter painter (this);                                           //一个类中的this表示一个指向该类自己的指针
    painter.setRenderHint(QPainter::Antialiasing);   /* 使用反锯齿(如果可用) */
    painter.translate(width()/2,height()/2);                /* 坐标变换为窗体中心 */
    int side = qMin(width(),height());
    painter.scale(side /200.0,side /200.0);                   /* 比例缩放 ,相当于把长宽都分成了相等的200份*/
    drawCrown(&painter);                                             /* 画表盘边框，表冠 */
    drawYawShow(&painter,nowYaw);                         /*先绘制磁罗盘，通过传递参数使罗盘转动*/
    drawRollNumericValue(&painter,nowRoll);                         /*再绘制横滚仪盘，仪盘不动，通过转指针来动*/
    drawBackground(&painter,nowRoll,nowPitch);                   /* 绘制内环*/
    drawPitchNumericValue(&painter,nowRoll,nowPitch);      /*绘制中间pitch的刻度尺*/
    drawTextPie(&painter,nowYaw,nowRoll,nowPitch,att_height);

}

void mySpeed::UpdateAngle()
{
  update();//刷新控件，会调用paintEvent函数
}

