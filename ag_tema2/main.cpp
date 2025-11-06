#include "stdafx.h"
#include "ag_tema2.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    ag_tema2 window;
    window.show();
    return app.exec();
}
