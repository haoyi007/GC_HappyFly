#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>
#include <QMovie>
#include <QtDebug>
namespace Ui {
class about;
}

class about : public QDialog
{
    Q_OBJECT

public:
    explicit about(QWidget *parent = 0);
    ~about();

private:
    Ui::about *ui;
    QMovie *movie;
private slots:
    void contrinueMovieSlot();
    void stopMovieSlot();
};

#endif // ABOUT_H
