#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QWidget>

class progressbar : public QWidget
{
    Q_OBJECT
private:
    enum{
        SIZEW = 400,
        SIZEH = 200,
    };
    void layout();
public:
    int total;
    int count;
    QString name;
    progressbar(QString,QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent *event);
public slots:
    void changeState();
    void display();
signals:
    void isClose();
};

#endif // PROGRESSBAR_H
