#include "catalog.h"
#include "button.h"
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <qDebug>

bool catalog::isOpen = false;

catalog::catalog(QWidget* parent)
    :QWidget(parent)
{
    isOpen = true;
    layout();
}

void catalog::closeEvent(QCloseEvent* )
{
    isOpen = false;
}

void catalog::layout()
{
    lineedit =  new QLineEdit();
    lineedit->setMinimumWidth(200);
    lineedit->setReadOnly(true);
    button = new QPushButton(".");
    updatebtn = new Button();
    hlayout = new QHBoxLayout();

    updatebtn->SetButtonIcons(QIcon(":/image/update.ico"),
                              QIcon(":/image/update.ico"),
                              QIcon(":/image/update.ico"));
    updatebtn->setFlat(true);
    label = new QLabel(QStringLiteral("当前路径："));
    hlayout->addWidget(updatebtn);
    hlayout->addWidget(label);
    hlayout->addWidget(lineedit);
    hlayout->addWidget(button);

    setLayout(hlayout);
    connect(button,SIGNAL(clicked()),this,SLOT(choose()));
}

void catalog::setPath(QString p)
{
    path = p;
    lineedit->setText(p);
}

QString catalog::getPath()
{
    return path;
}

void catalog::choose()
{
    QUrl url = QFileDialog::getExistingDirectoryUrl
            (this);

    lineedit->setText(url.toString().mid(8));
    path = url.toString().mid(8);

    if(path == "")return;
    emit(updatePath(path));
}
