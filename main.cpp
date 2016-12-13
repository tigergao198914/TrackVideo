#include <iostream>
#include <QApplication>
#include "main_window.h"
#include "video_file_source.h"
using namespace std;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
