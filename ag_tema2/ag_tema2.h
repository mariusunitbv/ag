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
    void drawUpdatedCells();
    void updateAlgorithmState();
    Qt::GlobalColor getNodeColor(Node node);

    Ui::ag_tema2Class ui;

    GridGraph m_graph;
    std::vector<GridGraph::NodePos_t> m_updatedCells;

    QTimer* m_updateTimer;

    static const int k_stepDelayMs;
};
