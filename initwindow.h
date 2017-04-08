#ifndef INITWINDOW_H
#define INITWINDOW_H

#include "premessage.h"
#include "addattrwindow.h"
#include "removeattrwindow.h"
#include <QWidget>

class QLabel;
class QPushButton;
class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QSpinBox;
class QComboBox;
class QLineEdit;

class initwindow : public QWidget
{
    Q_OBJECT
private:
    enum{
        VMAX = 1,
        HMAX = 8,
    };

    QLabel* preSet;
    QLabel* pixelSize;
    QLabel* wavelength;
    QLabel* startpoint;
    QLabel* endpoint;
    QLabel* plane;

    QLabel* image;

    map_t* map;
    QVector<QString>* namelist;
    addattrwindow* addattrWindow;
    removeattrwindow* removeattrWindow;

    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];

    void layout();
public:
    QPushButton* plusBtn;
    QPushButton* minusBtn;
    QComboBox* combobox;
    QLineEdit* pixelBox;
    QLineEdit* waveBox;
    QLineEdit* startBox;
    QLineEdit* endBox;
    QLineEdit* planeBox;
    QPushButton* okBtn;
    QPushButton* cancelBtn;
    initwindow(QVector<QString>* namelist,map_t* map,QWidget* parent = 0);
private slots:
    void setValue(int i);
    void minus();
    void plus();
    void updateStyle();
    void updateAttr(QString,premessage_t);
};

#endif // INITWINDOW_H
