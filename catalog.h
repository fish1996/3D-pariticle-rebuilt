#ifndef CATALOG_H
#define CATALOG_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QLabel;
class Button;

class catalog : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* lineedit;
    QPushButton* button;
    QHBoxLayout* hlayout;
    QLabel* label;
    QString path;
    void layout();
private slots:
    void choose();
public:
    static bool isOpen;
    Button* updatebtn;
    QString getPath();
    void setPath(QString);
    catalog(QWidget* parent = 0);
signals:
    void updatePath(QString);
protected:
    void closeEvent(QCloseEvent* event);
};

#endif // CATALOG_H
