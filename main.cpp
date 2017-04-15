
#include <QApplication>
#include "myhelper.h"
#include "thread.h"

int main(int argc, char *argv[])
{

    /*Engine *ep;
    if (!(ep=engOpen(NULL))) //测试是否启动Matlab引擎成功。
    {
        //qDebug() <<"Can't start Matlab engine!" <<endl;
        exit(1);
    }
    qDebug()<<"Open Matlab Engine!" <<endl;*/
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/main.ico"));
    thread th;
    myHelper::SetUTF8Code();
 //   myHelper::SetStyle("black");//黑色风格
//    myHelper::SetStyle("blue");//蓝色风格
 //   myHelper::SetStyle("gray");//灰色风格
//    myHelper::SetStyle("navy");//天蓝色风格
    myHelper::SetChinese();
    return a.exec();
}
