#include "mainwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowTitle("Catch figures!");
    w.setGeometry(100, 100, 1920, 1040);
    w.show();
    w.setMouseTracking(true);
    w.setCursor(QCursor(Qt::BlankCursor));

    return a.exec();
}
