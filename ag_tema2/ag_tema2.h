#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ag_tema2.h"
#include "graph.h"

class ag_tema2 : public QMainWindow {
    Q_OBJECT

   public:
    ag_tema2(QWidget* parent = nullptr);
    ~ag_tema2();

    void paintEvent(QPaintEvent* event) override;

   private:
    void drawGrid();
    void updateAlgorithmState();

    Ui::ag_tema2Class ui;

    GridGraph m_graph;
    bool m_bfsFinished;

    static const int k_stepDelayMs;
};
