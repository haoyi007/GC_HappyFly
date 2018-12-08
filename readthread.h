#ifndef READTHREAD_H
#define READTHREAD_H


#include <QThread>
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <qfiledialog>
#include <QSettings>
#include <qdebug.h>
#include <QTextCodec>
#include "ui_mainwindow.h"
#include <QFile>

class readThread : public QThread
{
    Q_OBJECT
public:
    readThread( QObject *parent);
    ~readThread();
    void run();
    void stopThread();
    void HAOYI_Solve_Data(QByteArray data_buf,quint8 num);
    int rcvbuf(char);


public Q_SLOTS:
    void readRecDataSlot(QByteArray readbuf);

Q_SIGNALS:
    void sendATIOk(float,float,float,float);

private:
    int readStartTmp;
    QByteArray readbuffer;
};

#endif // READTHREAD_H
