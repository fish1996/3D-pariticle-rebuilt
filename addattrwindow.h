//5530
#ifndef ADDATTRWINDOW_H
#define ADDATTRWINDOW_H

#include <QWidget>
#include "premessage.h"
class QLabel;
class QLineEdit;
class QCheckBox;
class QSpinBox;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class addattrwindow : public QWidget
{
    Q_OBJECT
private:
    enum{
        VMAX = 1,
        HMAX = 10,
    };

    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];

    QLabel* name;
    QLabel* zmin;
    QLabel* zmax;
    QLabel* interval;
    QLabel* lamda;
    QLabel* Mag;
    QLabel* dpix;
    QLabel* detection;
    QLabel* path;
    QLabel* minRadius;
    QLabel* maxRadius;
    QLabel* plotnum;

    QPushButton* choosePathBtn;

    void layout();
    bool isValid(const premessage_t& msg);
public:
    QLineEdit* nameText;
    QLineEdit* zminText;
    QLineEdit* zmaxText;
    QLineEdit* intervalText;
    QLineEdit* lamdaText;
    QLineEdit* MagText;
    QLineEdit* dpixText;
    QLineEdit* d1Text;
    QLineEdit* d2Text;
    QLineEdit* d3Text;
    QLineEdit* locationText;
    QLineEdit* pathText;

    QSpinBox* minRadiusText;
    QSpinBox* maxRadiusText;
    QSpinBox* plotnumText;

    QCheckBox* isInverse;
    QPushButton* okBtn;
    QPushButton* cancelBtn;
    addattrwindow(QWidget* parent = 0);
private slots:
    void addAttr();
    void cancelAddAttr();
signals:
    void updateAttr(QString name,premessage_t msg);
};

#endif // ADDATTRWINDOW_H
