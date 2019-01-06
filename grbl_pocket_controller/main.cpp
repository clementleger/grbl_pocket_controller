#include "grblpocketcontroller.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GrblPocketController w;
    w.show();

    return a.exec();
}
