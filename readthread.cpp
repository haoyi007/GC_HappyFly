#include "readthread.h"
//readthread功能应用是读取解析保存在文本中的数据
readThread::readThread(QObject *parent)
    :QThread(parent)
{

}
readThread::~readThread()
{
   stopThread();
    //执行delete时会调用析构函数
}


void readThread::stopThread()
{

}

QByteArray prodata3;


void readThread::run()
{
    if(readStartTmp==1)
    {
        qDebug()<<"读取线程已经开启";
        //帧读取：读取对象为已经存入文档的完整数据帧
        for(int i = 0; i < readbuffer.length(); i++)
        {
            if(rcvbuf(readbuffer[i]) == 10)//这里可以尝试用另外一个类的对应的接收解析的函数，获取完整数据帧prodata1
            {//上函数中有对prodata3进行附值
//                qDebug()<<"已经解析到完整数据";
//                if(prodata3[prodata3[3]+5] == char(0xaa))//对时间帧解析
//                {
//                    unsigned short s= (prodata3[4] & 0xff) | (prodata3[5] & 0xff) << 8;
//                    qDebug()<<"时间帧解析正常";
//                    qDebug()<<s;        //参数为ms

//                    this->sleep((0.0001*s));      //这里睡眠对主线程应该没有影响。

//                }
                HAOYI_Solve_Data(prodata3,prodata3[3]);    //数据解析显示

            }
        }
        readStartTmp=0;
        qDebug()<<"读取完毕";
    }

}

QByteArray copybuf1;
QByteArray prodata2;

void readThread::readRecDataSlot(QByteArray readbuf)//需要主线程将其传递过啦
{
    readStartTmp=1;
    readbuffer=readbuf;
    qDebug()<<"已经接收到需要读取的数据";
}


QByteArray RxBuffer_read;
int readThread::rcvbuf(char R_data)//读数据时进行的协议解析，比正常的只多接收8个时间帧数据
{

    static quint8 _data_len = 0,_data_cnt = 0;
    static quint8 state = 0;

    if(state==0&&R_data==(char)0xAA)
    {
        state=1;
        RxBuffer_read[0]=R_data;
        return 1;
    }
    else if((state==1)&&(R_data==(char)0xAA))
    {
        state=2;
        RxBuffer_read[1]=R_data;
        return 2;
    }
    else if(state==2&&(R_data==(char)0X01))
    {
        state=3;
        RxBuffer_read[2]=R_data;
        return 3;
    }
    else if(state==3&&R_data<60)
    {
        state = 4;
        RxBuffer_read[3]=R_data;
        _data_len = R_data+8; //数据长度,包含时间帧的长度
        _data_cnt = 0;
        return 4;
    }
    else if(state==4&&_data_len>0)
    {
        _data_len--;
        RxBuffer_read[4+_data_cnt++]=R_data;  //存储数据
        if(_data_len==0){state = 5;}
        return 5;
    }
    else if(state==5)
    {
        state = 0;
        RxBuffer_read[4+_data_cnt]=R_data; //应该是最后一个数据
        prodata3			= RxBuffer_read;
        RxBuffer_read.clear();
        return 10;
    }
    else
        state = 0;
         return 0;
}

void readThread::HAOYI_Solve_Data(QByteArray data_buf,quint8 num)
{
    quint8 sum = 0;
    float commRoll,commPitch,commYaw,current_Yaw;
    for(qint8 i=0;i<(num-1);i++)
        sum += data_buf[i];
    if(!(sum==data_buf[num-1]))		return;		//判断sum
    if(!(data_buf[0]==(char)0xAA && data_buf[1]==(char)0xAA))		return;		//判断帧头

    if(data_buf[2]==(char)0X01)   //接收姿态信息
    {
        //这里存在一个量纲的变化
        commRoll=0.01*((qint16)(data_buf[4]<< 8 | data_buf[5]));
        commPitch=0.01*((qint16)(data_buf[6]<< 8 | data_buf[7]));
        commYaw=0.01*((qint16)(data_buf[8]<< 8 | data_buf[9]));
        current_Yaw=10;
        Q_EMIT sendATIOk(commRoll,commPitch,commYaw,current_Yaw);
        qDebug()<<"数据帧解析正常";
    }

}
