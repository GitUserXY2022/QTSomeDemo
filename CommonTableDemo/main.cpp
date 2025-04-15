#include "choosetable.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChooseTable w;
    w.show();

    return a.exec();
}
