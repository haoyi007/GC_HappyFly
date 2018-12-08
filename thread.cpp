#include "thread.h"
#include "mainwindow.h"
#include<qdebug.h>
#include<global.h>
//这里读取串口数据，并进行解析
Thread::Thread(QObject *parent)
    :QThread(parent)
{
    qDebug()<<"begin";
     myCom= new QSerialPort();
     stopped = 1;
}
Thread::~Thread()
{
   stopThread();
   delete myCom;
    //执行delete时会调用析构函数
}
void Thread::run()
{
    QByteArray tempStr;
    while(stopped)
    {
        qDebug()<<"running";
        if(myCom->isOpen())
        {
            myTime				= new QTimer();//这里忘记了多久开启一次定时器了
            connect(myTime, SIGNAL(timeout()), this, SLOT(rcvdata()));//也进行定时读取数据
            connect(myCom, SIGNAL(readyRead()), this, SLOT(read_Data()));      //从这里开始两套接收协议
        }
    }
}
void Thread::stopThread()
{
    stopped = 0;
    QByteArray temp =myCom->readAll();//清除缓冲区数据
    if(myCom->isOpen()) {
               myCom->close();
           }
}

//串口发出readyRead信号后，进行read_Data槽函数
void Thread::read_Data()
{
    int 			count = myCom->bytesAvailable();

    if (count > 20)//这里数值太大会限制显示的频率
    {
        databuf 			= myCom->readAll();
        rcvdata();	//数据量超过20字节执行读取操作
    }
}

void Thread::rcvdata()
{
    dataprocess(databuf);//内含协议接收、写数据，全工程只有这里一处进行写操作，其中会发送更新显示的信号
    emit bytereceive(databuf);//发送读数据信号，串口初始化函数中进行了槽连接read_Data进行显示，方便核查协议

    databuf.clear();
}

 int IsSave=0;

void Thread::dataprocess(QByteArray buf)
{
    filewrites.clear();

    for (int i = 0; i < buf.length(); i++)
    {
        if (parserrcvbuf(buf[i]) == 10)//内含接收协议，该函数中对filewrites已经进行了赋值（须对此进行修改，协议匹配才可以进行写操作）
        {//暂时上函数中有数据解析显示
            if(IsSave==1)//开始保存数据
            {
                filewrite();//写数据内含文件名包含时间，数据文本流写入QDataStream用于二进制数据的快速读取
            }
            rcvbuffer			= filewrites;
            emit doprocess(rcvbuffer);//发送数据处理信号，串口初始化函数中进行了链接槽，数据更新显示，这里还没有完全对接上
            filewrites.clear();
        }
    }

}


int 			_p_rec_num = 0;
int 			totalnum = 0;
int 			_R_state = 0;
QByteArray		copybuf;
QByteArray		prodata;
QByteArray RxBuffer;
int 			count = 0;


int Thread::parserrcvbuf(char R_data)//这里需要进行协议匹配，并且搞好后return 10，对应的读取的协议也要改
{
    //该函数是目前使用的接收解析函数

    static quint8 _data_len = 0,_data_cnt = 0;
    static quint8 state = 0;

    if(state==0&&R_data==(char)0xAA)
    {
        state=1;
        RxBuffer[0]=R_data;
        return 1;
    }
    else if((state==1)&&(R_data==(char)0xAA))
    {
        state=2;
        RxBuffer[1]=R_data;
        return 2;
    }
    else if(state==2&&(R_data==(char)0X01||R_data==(char)0XC0))//帧类别
    {
        state=3;
        RxBuffer[2]=R_data;
        return 3;
    }
    else if(state==3&&R_data<60)
    {
        state = 4;
        RxBuffer[3]=R_data;    //数据长度，不包含前四个字节和最后一个校验和，纯数据字节长度
        _data_len = R_data;
        _data_cnt = 0;
        return 4;
    }
    else if(state==4&&_data_len>0)
    {
        _data_len--;
        RxBuffer[4+_data_cnt++]=R_data;  //存储数据
        if(_data_len==0){state = 5;}
        return 5;
    }
    else if(state==5)
    {
        state = 0;
        RxBuffer[4+_data_cnt]=R_data; //应该是最后一个数据（校验和）
        HAOYI_DT_Data_Receive_Anl(RxBuffer,_data_cnt+5);//数据解析显示
        filewrites			= RxBuffer;
        RxBuffer.clear();
        return 10;//解析成功
    }
    else
        state = 0;
         return 0;

}



int save_tmp=0;
QString 		dir;
void Thread::filewrite()       //目录，名称
{
        if(save_tmp==0)//重新保存数据时需要将其置0
        {
            dir += "D:/haoyi007-data/";
            dir+="haoyi007-data-";
            QDateTime		time = QDateTime::currentDateTime();
            dir+=time.toString("yyyy-MM-dd-hh-mm");
            dir 				+= ".dat";
            save_tmp=1;
        }
        QFile file(dir);
         qDebug()<<dir;
        file.open(QIODevice::Append);//以追加的方式打开，新增加的内容将被追加到文件末尾

        if (file.isOpen())
        {
            filewrites.append(settimeint());//这里是应该是后加
            QDataStream out(&file);
            out << filewrites;      //写入文件
        }
        file.close();
}

QByteArray Thread::settimeint()
{
    QByteArray		timebuf;
    QDateTime		time = QDateTime::currentDateTime();
    QString 		str = time.toString("zzz");

    this_time			= str.toShort();
    unsigned short	stime = this_time - last_time;//上次接收一帧数据与这次之间的时间间隔

    last_time			= this_time;
    timebuf[0]			= char(0x24);
    timebuf[1]			= char(0x01);
    timebuf[2]			= char(0xaa);
    timebuf[3]			= 0;
    timebuf[4]			= char(stime & 0xff);
    timebuf[5]			= char((stime >> 8) & 0xff);
    timebuf[6]			= 1;
    timebuf[7]			= 0;
    qint8			sum = 0;

    for (int i = 0; i < 7; i++)
        sum += qint8(timebuf[i]);

    timebuf[8]			= char(sum);
    return timebuf;

}

void Thread::saveStartSlot()//保存原有数据并以新文件开始记录数据
{
    save_tmp=0;//目录更新
    dir="";
    IsSave=1;
}

void Thread::saveStopSlot()//不保存数据
{
    IsSave=0;
}


/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用

void Thread::HAOYI_DT_Data_Receive_Anl(QByteArray data_buf,quint8 num)
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
    }
    else if(data_buf[2]==(char)0XC0)//接收激光雷达数据
    {
            //*(data_buf+4)=num 是第几组数据
           for(qint8 i=0;i<8;i++){
                URGDataTemp[8 * data_buf[4]+i].angle=(float)(0.01f*(quint16)( (data_buf[5+4*i] << 8) |data_buf[6+4*i]));
                URGDataTemp[8 * data_buf[4]+i].distance=(float)1.0f*(quint16)( (data_buf[7+4*i] << 8) |data_buf[8+4*i]);
            }
           Q_EMIT sendURGData(&URGDataTemp[8*data_buf[4]],(int)data_buf[4]);
    }

}

void Thread::comThreadInit(QString portName,int baudRate)
{
      if(myCom->isOpen())
       {
            myCom->close();
        }
      stopped = 1;
      myCom->setPortName(portName);
      myCom->open(QIODevice::ReadWrite);
      myCom->setBaudRate(baudRate);
      myCom->setDataBits(QSerialPort::Data8);//数据位8位
      myCom->setParity(QSerialPort::NoParity) ;//无奇偶校验
      myCom->setStopBits(QSerialPort::OneStop);//停止位1位
      myCom->setFlowControl(QSerialPort::NoFlowControl);//无流控制
}


void Thread::sendDateSlot(char *TxData,int num)
{
    myCom->write(TxData,num);
    qDebug()<<"send aaaa";
}

