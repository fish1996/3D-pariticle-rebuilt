#ifndef SAVEWINDOW_H
#define SAVEWINDOW_H

#include <QWidget>
#include <QLabel>
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QLineEdit;
class QComboBox;
class QCheckBox;

class savewindow : public QWidget
{
    Q_OBJECT
private:
    enum {
        VMAX = 1,
        HMAX = 4,
        LMAX = 3
    };
    QHBoxLayout* hlayout[HMAX];
    QVBoxLayout* vlayout[VMAX];

    QLabel label[LMAX];

    QPushButton* okBtn;
    QPushButton* cancelBtn;
    QPushButton* chooseBtn;

    QLineEdit* pathLine;

    QComboBox* prefix;

    QCheckBox* rebuiltBox;
    QCheckBox* extendBox;
    QCheckBox* detectBox;

public:
    savewindow(QWidget* parent = 0);
};

#endif // SAVEWINDOW_H
