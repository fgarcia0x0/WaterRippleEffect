#include <QApplication>
#include <mainwindow.h>
#include <qnamespace.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();

    return app.exec();
}