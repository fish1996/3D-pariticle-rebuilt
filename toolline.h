#ifndef TOOLLINE_H
#define TOOLLINE_H

#include <QGroupBox>
#include <QHBoxLayout>
#include "attr.h"
#include "holdbutton.h"

class toolLine : public QGroupBox
{
    Q_OBJECT
private:
    QHBoxLayout* hlayout;
    void layout();
public:
    imageButton* chooseCatalogBtn;
    imageButton* lastBtn;
    imageButton* nextBtn;
    imageButton* lastpairBtn;
    imageButton* nextpairBtn;
    imageButton* chooseBackgroundBtn;
    imageButton* clearBtn;
    imageButton* allchooseBtn;
    toolLine(QGroupBox* parent = 0);
};

#endif // TOOLLINE_H
