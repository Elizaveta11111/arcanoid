#include "arcanoid.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Arcanoid w;
    w.setFixedSize(500, 600);
    w.show();
    return a.exec();
}
