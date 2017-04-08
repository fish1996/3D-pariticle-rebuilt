#include "tablewindow.h"
#include <QTableWidgetItem>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <fstream>
#include <QFileDialog>
#include <QString>
#include <qDebug>
#include <QTextEdit>

tablewindow::tablewindow(QTableWidget* parent):
    QTableWidget(parent)
{
    layout();
}

void tablewindow::layout()
{
    rownum = 13;
    colnum = 3;
    setRowCount(rownum);
    setColumnCount(colnum);
    for(int i=0;i<rownum;i++){
        for(int j=0;j<colnum;j++){
            QTableWidgetItem* myitem = new QTableWidgetItem("");
            setItem(i,j,myitem);
        }
    }
    menu = new QMenu();
    saveAsXLSAction = new QAction(this);
    saveAsCSVAction = new QAction(this);
    saveAsXLSAction->setText(QStringLiteral("另存为xls"));
    saveAsCSVAction->setText(QStringLiteral("另存为csv"));
    connect(saveAsCSVAction,SIGNAL(triggered()),this,SLOT(saveAsCSV()));
    connect(saveAsXLSAction,SIGNAL(triggered()),this,SLOT(saveAsXLS()));
}

void tablewindow::contextMenuEvent(QContextMenuEvent* event)
{
    menu->clear();
    menu->addAction(saveAsXLSAction);
    menu->addAction(saveAsCSVAction);

    menu->exec(QCursor::pos());
    event->accept();
}


void tablewindow::saveAsXLS()
{
    QTableWidget* table = this;

    table->horizontalHeaderItem(0);

    //构建html文档内容
    QString filepath = QFileDialog::getSaveFileName(this, tr("Save as..."),
            QString(), tr("EXCEL files (*.xls *.xlsx);;HTML-Files (*.htm *.html);;"));
    QString Begin = QString::fromLocal8Bit("<html><head></head><body><table border=\"1\" >");
    QString end = QString::fromLocal8Bit("</table></body></html>");
    QList<QString> list;
    int row = table->rowCount();
    int col = table->columnCount();
/*  //若有列标题，取消此部分注释
    QString header = "<tr>";
    for(int i=0;i<col;i++)
    {
        QString cel = table->horizontalHeaderItem(i)->text();
        header += QString("<td>%1</td>").arg(cel);
    }
    header += "</tr>";
    list.push_back(header);
*/
    for(int i=0;i<row;i++)
    {
        QString rowStr = "<tr>";
        for(int j=0;j<col;j++)
        {
            QString cel = table->item(i,j)->text();
            rowStr += QString("<td>%1</td>").arg(cel);
        }
        rowStr += "</tr>";
        list.push_back(rowStr);
    }
    QString text = Begin;
    for(int i=0;i<list.size();++i)
    {
        text.append(list.at(i));
    }
    text.append(end);
    QTextEdit textEdit;
    textEdit.setText(text);


    QFile file(filepath);
    if(file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts<<textEdit.document()->toHtml("UTF-8");
    }
}

