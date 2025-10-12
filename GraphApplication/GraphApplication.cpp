#include "stdafx.h"
#include "GraphApplication.h"

GraphApplication::GraphApplication(QWidget *parent) : QMainWindow(parent) {
    ui.setupUi(this);

    connect(ui.orientedRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) ui.drawingZone->SetGraphType(GraphType::ORIENTED);
    });

    connect(ui.unorientedRadioButton, &QRadioButton::toggled, this, [=](bool checked) {
        if (checked) {
            if (!ui.drawingZone->SetGraphType(GraphType::UNORIENTED)) {
                ui.orientedRadioButton->toggle();
            }
        }
    });

    connect(ui.actionMatrice_adiacenta, &QAction::triggered, this, [=]() {
        QString filePath = QDir::currentPath() + "\\graph.txt";
        if (QFile::exists(filePath)) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        } else {
            QMessageBox::information(this, "Informatie",
                                     "Programul inca nu a creat fisierul graph.txt!");
        }
    });
}

GraphApplication::~GraphApplication() {}
