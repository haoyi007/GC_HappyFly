#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <qfiledialog>
#include <QSettings>
#include <qdebug.h>
#include <QTextCodec>
#include "ui_mainwindow.h"
#include "global.h"
#include <QFile>

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread( QObject *parent);
    ~Thread();
    void run();
    void stopThread();
    void comThreadInit(QString portName,int baudRate);
    void HAOYI_DT_Data_Receive_Prepare(quint8 data);
    void HAOYI_DT_Data_Receive_Anl(QByteArray data_buf,quint8 num);
    float rec_lat,rec_lon;
    int parserrcvbuf(char);          //接收分类
    void save2ringbuf(QByteArray);          //存储数据
    void filewrite();
    RPlidar_Data_Typedef  URGDataTemp[256];
    QByteArray settimeint();
    unsigned short last_time ;
    unsigned short this_time;
   QTimer *myTime;

public Q_SLOTS:
    void sendDateSlot(char  *TxData,int  num);
    void read_Data();   //读取数据
    void dataprocess(QByteArray);     //帧提取
    void rcvdata();
    void saveStartSlot();
    void saveStopSlot();

Q_SIGNALS:
    void sendATIOk(float roll,float pitch,float yaw,float current_Yaw);
    void send_Now_Atti(float roll,float pitch,float yaw,float target_Yaw);
    void send_Air_Rec_Commatti(float roll,float pitch,float yaw);
    void sendURGData(RPlidar_Data_Typedef arr[],int whitchnum);
    void transReceiveData(QByteArray requestData);
    void transTargetPosition(float lat,float lon);
    void bytereceive(QByteArray);   //读数据信号
    void doprocess(QByteArray);     //数据处理

private:
    int stopped;
    QByteArray requestData;
    QByteArray databuf;
    QSerialPort *myCom;//串口变量
    QByteArray rcvbuffer;       //接收数据
    QByteArray filewrites;
    QByteArray filereads;

};

#endif // THREAD_H
