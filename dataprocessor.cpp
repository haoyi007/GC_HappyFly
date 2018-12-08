#include "dataprocessor.h"

DataProcessor::DataProcessor()
{
//    getName();

}

DataProcessor::~DataProcessor()
{

}

void DataProcessor::dataStart()
{
    this->start();
}

void DataProcessor::stop()
{
    this->terminate();
}

void DataProcessor::fileRead(QString path, int offset)
{
    QFile file(path);
    fileSize = file.size();
    int index = int(offset / 100.0 * fileSize);

    dataCount = 0;
    dataCount += offset;

    QByteArray buf;
    if(file.open(QIODevice::ReadOnly))
    {
        file.seek(index);
        buf = file.readAll();//读取所有内容
    }

    qDebug()<<tr("读取结束");
    file.close();

    emit parserdata(buf);//进行显示的相关解析操作
}


void DataProcessor::HAOYI_Solve_Data_Receive_Anl(QByteArray data_buf,quint8 num)
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

}


int DataProcessor::getFileindex()
{
    return fileIndex;
}

void DataProcessor::getName()
{
    QString dir = QCoreApplication::applicationDirPath();
    QDir qdir(dir);
    qdir.cdUp();
    dir = qdir.canonicalPath();

    dir += "/name.txt";

    QFile file(dir);
//    int i = 0;
    if(file.open(QIODevice::ReadOnly))
    {
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str = QString(line);
//            dataTable[i++] = str.replace("\r\n", "");
        }
    }

}
