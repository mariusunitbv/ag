#include "stdafx.h"
#include "GraphApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    GraphApplication window;
    window.show();
    return app.exec();
}
