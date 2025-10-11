#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_GraphApplication.h"

class GraphApplication : public QMainWindow
{
    Q_OBJECT

public:
    GraphApplication(QWidget *parent = nullptr);
    ~GraphApplication();

private:
    Ui::GraphApplicationClass ui;
};

