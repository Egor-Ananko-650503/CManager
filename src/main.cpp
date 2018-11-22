#include "cmanager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CManager w;
    w.show();

    return a.exec();
}
