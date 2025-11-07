#include "stdafx.h"
#include "ag_tema2.h"

#include <fstream>

const int ag_tema2::k_stepDelayMs = 400;

ag_tema2::ag_tema2(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);

    auto fileName = QFileDialog::getOpenFileName(nullptr, "Select map input file");
    std::ifstream inputFile{fileName.toStdString()};
    if (!inputFile) {
        std::exit(0);
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        m_graph.addRow();
        for (auto c : line) {
            if (isspace(c)) {
                continue;
            }

            const auto nodeType = static_cast<Node::NodeType>(c - '0');
            m_graph.addNode(nodeType);
        }
    }

    setAutoFillBackground(true);
    setPalette(QPalette{Qt::white});

    m_graph.initBFS();

#ifdef GRAPH_DRAW_INSTANTLY
    ag_tema2::updateAlgorithmState();
#else
    m_updateTimer = new QTimer();
    connect(m_updateTimer, &QTimer::timeout, this, &ag_tema2::updateAlgorithmState);
    m_updateTimer->start(k_stepDelayMs);
#endif
}

ag_tema2::~ag_tema2() { delete m_updateTimer; }

void ag_tema2::paintEvent(QPaintEvent* event) {
    if (event->rect() == rect()) {
        drawGrid();
    } else {
        drawUpdatedCells();
    }
}

void ag_tema2::drawGrid() {
    QPainter painter{this};

    const auto& [rows, columns] = m_graph.getGridDimensions();
    const int cellWidth = rect().width() / columns, cellHeight = rect().height() / rows;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            painter.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
            painter.fillRect(j * cellWidth + 1, i * cellHeight + 1, cellWidth - 1, cellHeight - 1,
                             getNodeColor(m_graph[{i, j}]));
        }
    }
}

void ag_tema2::drawUpdatedCells() {
    QPainter painter{this};

    const auto& [rows, columns] = m_graph.getGridDimensions();
    const int cellWidth = rect().width() / columns, cellHeight = rect().height() / rows;

    for (const auto& [i, j] : m_updatedCells) {
        painter.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
        painter.fillRect(j * cellWidth + 1, i * cellHeight + 1, cellWidth - 1, cellHeight - 1,
                         getNodeColor(m_graph[{i, j}]));
    }

    m_updatedCells.clear();
}

void ag_tema2::updateAlgorithmState() {
    auto lastUpdatedCell = m_graph.runBFSIteration();
    if (!lastUpdatedCell) {
        if (m_updateTimer) {
            m_updateTimer->stop();
        }

        QApplication::beep();
        update();

        return;
    }

    const auto& [rows, columns] = m_graph.getGridDimensions();
    const int cellWidth = rect().width() / columns, cellHeight = rect().height() / rows;

    m_updatedCells.push_back(lastUpdatedCell.value());
    update(lastUpdatedCell.value().second * cellWidth, lastUpdatedCell.value().first * cellHeight,
           cellWidth, cellHeight);
}

Qt::GlobalColor ag_tema2::getNodeColor(Node node) {
    switch (node.getType()) {
        case Node::NodeType::WALKABLE:
            return Qt::white;
        case Node::NodeType::START:
            return Qt::blue;
        case Node::NodeType::FINISH:
            return Qt::red;
        case Node::NodeType::CURRENTLY_ANALYZED:
            return Qt::magenta;
        case Node::NodeType::VISITED:
            return Qt::yellow;
        case Node::NodeType::ANALYZED:
            return Qt::green;
    }

    return Qt::black;
}
