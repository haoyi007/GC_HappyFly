#include "radiatrueshow.h"
#include "ui_radiatrueshow.h"
#include "mainwindow.h"
static int receiveOk=0;
static  int init_ok=0;
#define useDrawPoint  1   //使用画点功能为1
int test=0;

radiaTrueShow::radiaTrueShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::radiaTrueShow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags=Qt::Dialog;

    flags |=Qt::WindowMinimizeButtonHint;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    for (int i=0; i<256; i++)
    {
        pointf[i]=new QPointF;
    }
    pix=new QPixmap(ui->widget->size());
    radiaTimer=new QTimer ;
    connect( radiaTimer, &QTimer::timeout, this,&radiaTrueShow::doRadar);    //将定时器超时信号与槽(功能函数)联系起来
    radiaTimer->start(100);    //开始运行定时器，定时时间间隔100ms
//    if(init_ok==0||receiveOk==1){
//        doRadar();
//    }
//    connect(ui->connectURGDataPushButton,SIGNAL(clicked()),this,SLOT(connectURGData()));
}

radiaTrueShow::~radiaTrueShow()
{
    delete radiaTimer;
    delete ui;
    delete pix;
    for (int i=0; i<256; i++)
    {
        delete pointf[i];
    }
//   getnum=0;
    init_ok=0;
}
void radiaTrueShow::receiveNowAttiSlot2(float roll,float pitch,float yaw,float target_Yaw)
{
    now_roll_here=roll;
    now_pitch_here=pitch;
    now_yaw_here = yaw;
    now_taretyaw_here=target_Yaw;
}

void radiaTrueShow::doRadar()
{
    int i;
    pix->fill(Qt::white);
    QPainter painter(pix);
    QPainterPath pathTemp;
    painter.translate(ui->widget->rect().center().x(),ui->widget->rect().center().y());   //以矩形的中心为原点
    QRect rect;
    int lenght = ui->widget->rect().height()/20;
    QBrush brush(Qt::green, Qt::Dense2Pattern);  //画刷可以设置颜色和填充模式
    painter.setBrush(brush);
    painter.drawEllipse(-(lenght*10),-(lenght*10),lenght*20,lenght*20);
    painter.setPen(Qt::white);
    painter.setBrush(Qt::NoBrush);
    for(i = 0;i<10;i++)//绘制同心圆
    {
        painter.drawEllipse(-(lenght*i),-(lenght*i),(lenght*i)*2,(lenght*i)*2);//绘制椭圆、圆(x,y,w,h),,,(x,y)为左上角坐标，w为长，h为宽
    }
    //绘制辐射线
    painter.translate(-ui->widget->rect().center().x(),-ui->widget->rect().center().y());//变回原来的零点
    painter.setPen(Qt::red);
    painter.setBrush(Qt::NoBrush);

    pathTemp.moveTo(ui->widget->rect().center());
    pathTemp.lineTo(ui->widget->rect().center().x()+200,ui->widget->rect().center().y()+200);
    painter.drawPath(pathTemp);

    pathTemp.moveTo(ui->widget->rect().center());
    pathTemp.lineTo(ui->widget->rect().center().x()-200,ui->widget->rect().center().y()+200);
    painter.drawPath(pathTemp);

    painter.setPen(Qt::red);
    painter.drawText(20, 0, 100, 20,Qt::AlignHCenter | Qt::AlignTop, "雷达显示图");
    //painter.drawText(x, y, width, height, alignment, text);其中(x,y,width,height)所确定的矩形，alignment确定文字在矩形中的位置。
    painter.translate(ui->widget->rect().center().x(),ui->widget->rect().center().y());//变回图形中心
    painter.drawText(lenght*0.707, lenght*0.707, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "1m");
    painter.drawText(lenght*0.707*2, lenght*0.707*2, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "2m");
    painter.drawText(lenght*0.707*3, lenght*0.707*3, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "3m");
    painter.drawText(lenght*0.707*4, lenght*0.707*4, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "4m");
    painter.drawText(lenght*0.707*5, lenght*0.707*5, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "5m");
    painter.drawText(lenght*0.707*6, lenght*0.707*6, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "6m");
    painter.drawText(lenght*0.707*7, lenght*0.707*7, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "7m");
    painter.drawText(lenght*0.707*8, lenght*0.707*8, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "8m");
    painter.drawText(lenght*0.707*9, lenght*0.707*9, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "9m");
    painter.drawText(lenght*0.707*10, lenght*0.707*10, 20, 20,Qt::AlignHCenter | Qt::AlignTop, "10m");

    if(init_ok==0) {//少去重复计算三角函数
        init_ok=1;
        painter.setPen(QPen(Qt::black, 2));
        painter.setBrush(Qt::NoBrush);
        for (int i=0; i<256; i++)//绘制216个激光雷达数据点
        {
            pointf[i]->setX(0.0264*RPlidar_Body_Data[i].distance*qCos((RPlidar_Body_Data[i].angle)/360*2*3.14));//这里单位系数大小不知道是否合适
            pointf[i]->setY(-0.0264*RPlidar_Body_Data[i].distance*qSin((RPlidar_Body_Data[i].angle)/360*2*3.14));
            if(test)
            {
                pointf[0]->setX(0.0264*1000);
                pointf[0]->setY(0);
            }
            painter.drawPoints(pointf[i],1);
        }

        painter.setPen(QPen(Qt::red, 3));  //目标yaw方向标红
        painter.setBrush(Qt::VerPattern);
        painter.drawLine(0,0, -80*qCos((draw_yaw-now_yaw_here+90)/360*2*3.14), -80*qSin((draw_yaw-now_yaw_here+90)/360*2*3.14));
        painter.setPen(QPen(Qt::black, 3));  //目标roll方向标黑(目标命令左负右正)
        painter.setBrush(Qt::VerPattern);
        painter.drawLine(0,0,26.4*draw_roll, 0);
        painter.setPen(QPen(Qt::blue, 3));  //目标pitch方向标蓝(目标命令前正后负)
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0,0,0, -26.4*draw_pitch);
        update();
    }
    else {
            painter.setPen(QPen(Qt::black, 2));
            painter.setBrush(Qt::NoBrush);
            if(receiveOk==1)
             {
                    for (int i=0; i<256; i++)//绘制216个激光雷达数据点
                    {
                        pointf[i]->setX(0.0264*RPlidar_Body_Data[i].distance*qCos((RPlidar_Body_Data[i].angle)/360*2*3.14));//这里单位系数大小不知道是否合适
                        pointf[i]->setY(-0.0264*RPlidar_Body_Data[i].distance*qSin((RPlidar_Body_Data[i].angle)/360*2*3.14));

                     }
                     receiveOk=0;

                    if(test)
                    {
                        pointf[0]->setX(0.0264*1000);
                        pointf[0]->setY(0);
                    }

             }
            for(int j=0;j<256;j++)
            {
                 painter.drawPoints(pointf[j],1);
            }
             painter.setPen(QPen(Qt::red, 3));  //目标方向标红
             painter.setBrush(Qt::VerPattern);
             painter.drawLine(0,0, -80*qCos((draw_yaw-now_yaw_here+90)/360*2*3.14), -80*qSin((draw_yaw-now_yaw_here+90)/360*2*3.14));
             painter.setPen(QPen(Qt::black, 3));  //目标roll方向标黑(目标命令左负右正)
             painter.setBrush(Qt::VerPattern);
             painter.drawLine(0,0,26.4*draw_roll, 0);
             painter.setPen(QPen(Qt::blue, 3));  //目标pitch方向标蓝(目标命令前正后负)
             painter.setBrush(Qt::NoBrush);
             painter.drawLine(0,0,0,-26.4*draw_pitch);
             update();
        }
}

void radiaTrueShow::receiveURGDataSlot2(RPlidar_Data_Typedef arr[],int num)//这里是对应的接收激光雷达的槽函数
{
//       qDebug()<<"receiveURGData"<<num<<"is"<<data;
       for(qint8 i=0;i<8;i++)
       {
            RPlidar_Body_Data[8*num+i].angle=arr[8*num+i].angle;//赋值更新了
            RPlidar_Body_Data[8*num+i].distance=arr[8*num+i].distance;
       }
       if(num%4==1)receiveOk=1;//根据电脑运行情况进行修改
//       receiveNum[getnum]=num;
//       getnum++;
}

void radiaTrueShow::receiveATISlot2(float roll, float pitch, float yaw, float current_Yaw)
{
    draw_roll=roll;
    draw_pitch=pitch;
    draw_yaw=yaw;
    draw_current_Yaw=current_Yaw;
}

void radiaTrueShow::paintEvent(QPaintEvent *event)//双缓冲在计算机图形学中，双缓冲是一种画图技术，使用这种技术可以使得画图没有（至少是减少）闪烁、撕裂等不良效果，并减少等待时间。
//双缓冲机制的原理大概是：所有画图操作将它们画图的结果保存在一块系统内存区域中，这块区域通常被称作“后缓冲区（back buffer）”，当所有的绘图操作结束之后，将整块区域复制到显示内存中，
//这个复制操作通常要跟显示器的光栈束同步，以避免撕裂。双缓冲机制必须要求有比单缓冲更多的显示内存和CPU消耗时间，因为“后缓冲区”需要显示内存，而复制操作和等待同步需要CPU时间。
{

    QPainter painter1(this);
    painter1.drawPixmap(QPoint(0,0),*pix);

}

void radiaTrueShow::connectURGData()
{
     if(ui->connectURGDataPushButton->text() == QString("连接雷达数据"))
     {
         ui->connectURGDataPushButton->setText("断开雷达数据");

             quint8 _cnt=0;
             char  temp=11;

             data_to_send[_cnt++]=0xAA;
             data_to_send[_cnt++]=0xAA;
             data_to_send[_cnt++]=0x05;
             data_to_send[_cnt++]=0;

             data_to_send[_cnt++]=temp;

             data_to_send[3] = _cnt-4;

             quint8 sum = 0;
             for(quint8 i=0;i<_cnt;i++)
                 sum += data_to_send[i];

             data_to_send[_cnt++]=sum;
         Q_EMIT senddata(data_to_send,_cnt);
     }
     else
     {
          ui->connectURGDataPushButton->setText("连接雷达数据");
          quint8 _cnt=0;
          char  temp=31;

          data_to_send[_cnt++]=0xAA;
          data_to_send[_cnt++]=0xAA;
          data_to_send[_cnt++]=0x05;
          data_to_send[_cnt++]=0;

          data_to_send[_cnt++]=temp;

          data_to_send[3] = _cnt-4;

          quint8 sum = 0;
          for(quint8 i=0;i<_cnt;i++)
              sum += data_to_send[i];

          data_to_send[_cnt++]=sum;
      Q_EMIT senddata(data_to_send,_cnt);
     }

}
