#include "stdafx.h"
#include "GraphApplication.h"

GraphApplication::GraphApplication(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    connect(ui.orientedRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) ui.drawingZone->RadioOptionSelected(1);
    });

    connect(ui.unorientedRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) ui.drawingZone->RadioOptionSelected(2);
    });
}

GraphApplication::~GraphApplication() {}
