#pragma once

#include <qwidget.h>

#include "Graph.h"

class DrawingZone : public QWidget {
    Q_OBJECT

   public:
    DrawingZone(QWidget* parent = nullptr);

    bool SetGraphType(GraphType graphType);

   protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

   private:
    void DrawNodes(QPainter* painter) const;
    void DrawEdges(QPainter* painter) const;

    Graph m_graph;
    QFont m_regularFont, m_smallFont;

    QPoint m_startDragPosition, m_initialPosition;
};
