#include "stdafx.h"
#include "DrawingZone.h"

DrawingZone::DrawingZone(QWidget* parent)
    : m_regularFont("Consolas", 14), m_smallFont("Consolas", 8) {
    setFocusPolicy(Qt::ClickFocus);

    m_graph.SetOrientedGraph(true);
}

bool DrawingZone::SetGraphType(GraphType graphType) {
    switch (graphType) {
        case GraphType::ORIENTED:
            m_graph.SetOrientedGraph(true);
            break;
        case GraphType::UNORIENTED:
            if (m_graph.GetEdges().size() > 1) {
                const auto reply =
                    QMessageBox::warning(this, "Atentie",
                                         "Unele arce ar putea fi sterse, pentru a putea face "
                                         "graful neorientat.\nContinuam?",
                                         QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::Yes) {
                    m_graph.MakeGraphUnoriented();
                } else {
                    this->setFocus();
                    return false;
                }
            }

            m_graph.SetOrientedGraph(false);
            break;
    }

    this->setFocus();
    update();

    m_graph.SaveGraph();
    return true;
}

void DrawingZone::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    const QPoint mousePos = event->pos();
    Node* nearestNode = m_graph.GetNearestNode(mousePos);
    if (nearestNode) {
        const QPoint nodePos = nearestNode->GetCenter();

        m_graph.SetDraggedNode(nearestNode);
        m_initialPosition = nodePos;
        m_startDragPosition = mousePos - nodePos;

        setCursor(Qt::ClosedHandCursor);
    }
}

void DrawingZone::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) {
        return;
    }

    const QPoint mousePos = event->pos();
    Node* movedNode = m_graph.GetDraggedNode();
    if (movedNode) {
        const QPoint nodePos = movedNode->GetCenter();

        unsetCursor();

        m_graph.SetDraggedNode(nullptr);
        if (nodePos != m_initialPosition) {
            return;
        }
    }

    Node* nearestNode = m_graph.GetNearestNode(mousePos);

    if (nearestNode) {
        m_graph.ToggleSelectNode(nearestNode);
    } else if (!nearestNode && m_graph.CanPlaceNode(mousePos)) {
        m_graph.AddNode(mousePos);
    }

    update();
}

void DrawingZone::mouseMoveEvent(QMouseEvent* event) {
    Node* movedNode = m_graph.GetDraggedNode();
    if (!movedNode) {
        return;
    }

    const QPoint newPosition = event->pos() - m_startDragPosition;
    if (!m_graph.CanDragNodeTo(movedNode, newPosition)) {
        return;
    }

    m_graph.DragNode(movedNode, newPosition);
    update();
}

void DrawingZone::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Return:
            m_graph.ConnectNodes();
            update();
            break;
        case Qt::Key_End:
            m_graph.TerminateEdgesOfSelectedNodes();
            update();
            break;
        case Qt::Key_Delete:
            m_graph.DeleteSelectedNodes();
            update();
            break;
        case Qt::Key_Home:
            m_graph.DeselectAllNodes();
            update();
            break;
        case Qt::Key_PageUp:
            m_graph.DeselectAllNodes();
            m_graph.SelectNodesAscending(0, m_graph.GetNodes().size());
            update();
            break;
        case Qt::Key_PageDown:
            m_graph.DeselectAllNodes();
            m_graph.SelectNodesDescending(m_graph.GetNodes().size() - 1, 0);
            update();
            break;
    }
}

void DrawingZone::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);
    painter.setFont(m_regularFont);

    DrawNodes(&painter);
    DrawEdges(&painter);
}

void DrawingZone::DrawNodes(QPainter* painter) const {
    const auto& nodes = m_graph.GetNodes();
    for (auto node : nodes) {
        painter->setPen(Qt::black);

        if (node->IsSelected()) {
            painter->setFont(m_smallFont);
            painter->drawText(node->GetSelectedIndexDrawPosition(), Qt::AlignCenter,
                              QString::number(node->GetSelectedIndex() + 1));
            painter->setFont(m_regularFont);

            painter->setPen(Qt::darkGreen);
        }

        painter->drawEllipse(node->GetCenter(), Node::kNodeRadius, Node::kNodeRadius);
        painter->drawText(node->GetRectangle(), Qt::AlignCenter,
                          QString::number(node->GetIndex() + 1));
    }
}

void DrawingZone::DrawEdges(QPainter* painter) const {
    const auto& edges = m_graph.GetEdges();
    for (const auto& edge : edges) {
        painter->setPen(Qt::black);

        Node *source = edge.GetSource(), *target = edge.GetTarget();

        const QPoint srcCenter = source->GetCenter();
        const QPoint targetCenter = target->GetCenter();

        const QPoint direction = targetCenter - srcCenter;
        const double distanceBetweenNodes = std::hypot(direction.x(), direction.y());
        const QPoint radiusOffset = direction * (Node::kNodeRadius / distanceBetweenNodes);

        const QPoint lineStart = srcCenter + radiusOffset;
        const QPoint lineEnd = targetCenter - radiusOffset;

        painter->drawLine(lineStart, lineEnd);
        if (m_graph.IsOrientedGraph()) {
            const double angle = std::atan2(-direction.y(), direction.x());
            const QPoint arrowP1 = lineEnd - QPoint(sin(angle + M_PI / 3) * Graph::kEdgeArrowSize,
                                                    cos(angle + M_PI / 3) * Graph::kEdgeArrowSize);
            const QPoint arrowP2 =
                lineEnd - QPoint(sin(angle + M_PI - M_PI / 3) * Graph::kEdgeArrowSize,
                                 cos(angle + M_PI - M_PI / 3) * Graph::kEdgeArrowSize);
            const QPolygon arrowHead{lineEnd, arrowP1, arrowP2};

            painter->setBrush(edge.IsSelected() ? Qt::green : Qt::black);
            painter->drawPolygon(arrowHead);
        }
    }
}
