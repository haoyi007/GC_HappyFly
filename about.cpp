#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);
    this->movie= new QMovie("gaoguai.gif");
    ui->movieLabel->setMovie(this->movie);
    qDebug()<<"current movie has"<<this->movie->frameCount();
    this->movie->start();
    QObject::connect(ui->contrinueButton,SIGNAL(clicked()),this,SLOT(contrinueMovieSlot()));
    QObject::connect(ui->stopButton,SIGNAL(clicked()),this,SLOT(stopMovieSlot()));
}

about::~about()
{
    delete ui;
}
void about::contrinueMovieSlot()
{
    this->movie->start();
}
void about::stopMovieSlot()
{
    this->movie->stop();
}
