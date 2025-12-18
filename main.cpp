#include "masterview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MasterView w;

    w.resize(500, 300);  // 初始大小
    w.show();
    return a.exec();
}
