#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setTitle("Catch figures!");
    w.setGeometry(100, 100, 800, 600);
    w.show();
//    w.setMouseTracking(true);
    w.setCursor(QCursor(Qt::BlankCursor));

    return a.exec();
}
