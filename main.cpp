
#include <QApplication>
#include "myhelper.h"
#include "thread.h"

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/main.ico"));
    thread th;
    myHelper::SetUTF8Code();

    myHelper::SetChinese();
    return a.exec();
}
