#include "emaildialog.h"
#include <QApplication>
#include <QFile>
#include <loginwidget.h>
#include "emaildialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   LoginWidget w;
    w.show();
    return a.exec();
}
