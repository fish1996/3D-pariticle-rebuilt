
#include <QApplication>
#include "thread.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/main.ico"));
    thread th;
    return a.exec();
}
