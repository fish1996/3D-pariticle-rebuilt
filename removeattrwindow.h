#ifndef REMOVEATTRWINDOW_H
#define REMOVEATTRWINDOW_H

#include "premessage.h"
#include <QWidget>

class QComboBox;
class QLabel;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class removeattrwindow : public QWidget
{
    Q_OBJECT
private:
    enum{
        HMAX = 1,
        VMAX = 1,
    };
    QLabel* tips;

    QVector<QString>* namelist;
    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];
    map_t* map;
    void layout();

public:
    QPushButton* okBtn;
    QPushButton* cancelBtn;
    QComboBox* attrBox;
    removeattrwindow(map_t* m,QVector<QString>* namelist,QWidget* parent = 0);
private slots:
    void cancelRemoveAttr();
    void removeAttr();
signals:
    void updateStyle();
};

#endif // REMOVEATTRWINDOW_H
