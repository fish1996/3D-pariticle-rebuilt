#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QTableWidget>

class QTableWidgetItem;
class QMenu;
class QAction;

class tablewindow : public QTableWidget{
    Q_OBJECT
private:
    int rownum;
    int colnum;

    QMenu* menu;
    QAction* saveAsXLSAction;
    QAction* saveAsCSVAction;

    void layout();
public:
    tablewindow(QTableWidget* parent = 0);
protected:
    void contextMenuEvent(QContextMenuEvent* event);
private slots:
    void saveAsXLS();
    void saveAsCSV();
};

#endif // TABLEWINDOW_H
