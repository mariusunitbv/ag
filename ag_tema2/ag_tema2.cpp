#include "stdafx.h"
#include "ag_tema2.h"

#include <fstream>

const int ag_tema2::k_stepDelayMs = 1;

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
    QTimer::singleShot(100, this, &ag_tema2::updateAlgorithmState);
}

ag_tema2::~ag_tema2() {}

void ag_tema2::paintEvent(QPaintEvent* event) {
    QPainter painter{this};

    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid();
}

void ag_tema2::drawGrid() {
    QPainter painter{this};

    const auto windowWidth = rect().width(), windowHeight = rect().height();

    const auto& grid = m_graph.getGrid();
    auto [rows, columns] = m_graph.getGridDimensions();
    int cellWidth = windowWidth / columns, cellHeight = windowHeight / rows;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            const QColor cellColor = [=]() {
                Node* node = m_graph[{i, j}];
                if (node) {
                    switch (node->getType()) {
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
                }
                return Qt::black;
            }();

            painter.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight);
            painter.fillRect(j * cellWidth + 1, i * cellHeight + 1, cellWidth - 1, cellHeight - 1,
                             cellColor);
        }
    }
}

void ag_tema2::updateAlgorithmState() {
    if (m_bfsFinished) {
        return;
    }

    if (!m_graph.runBFSIteration()) {
        m_bfsFinished = true;
        QApplication::beep();
    }

    update();
    QTimer::singleShot(k_stepDelayMs, this, &ag_tema2::updateAlgorithmState);
}
