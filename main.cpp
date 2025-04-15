#include "UI/MainWindow/mainwindow.h"
#include <QApplication>
#include <QDirIterator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "===== 可用资源列表 =====";
    QDirIterator it(":", QDirIterator::Subdirectories);
    while(it.hasNext()) {
        qDebug() << it.next();
    }
    qDebug() << "=======================";

    MainWindow w;
    w.show();
    return a.exec();
}
