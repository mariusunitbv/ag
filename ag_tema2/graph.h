#pragma once
#include <queue>

// #define GRAPH_DRAW_INSTANTLY

class Node {
   public:
    enum class NodeType : uint8_t {
        WALL = 0,
        WALKABLE,
        FINISH,
        START,
        CURRENTLY_ANALYZED,
        VISITED,
        ANALYZED
    };

    explicit Node(NodeType type) : m_type(type) {}

    NodeType getType() const { return m_type; }
    void setType(NodeType type) { m_type = type; }

   private:
    NodeType m_type;
};

class GridGraph {
   public:
    using NodePos_t = std::pair<size_t, size_t>;

    Node& operator[](const NodePos_t& p) { return m_nodesMatrix[p.first][p.second]; }

    void addNode(Node::NodeType nodeType) {
        auto& lastRow = m_nodesMatrix.back();

        lastRow.emplace_back(nodeType);
        if (nodeType == Node::NodeType::START) {
            m_startNode = std::make_pair(m_nodesMatrix.size() - 1, lastRow.size() - 1);
        }
    }

    void addRow() { m_nodesMatrix.emplace_back(); }

    auto getGridDimensions() const {
        return std::make_pair(m_nodesMatrix.size(), m_nodesMatrix.back().size());
    }

    void initBFS() {
        size_t nodesCount = m_nodesMatrix.size() * m_nodesMatrix.back().size();
        size_t startNodeIdx = m_startNode.first * m_nodesMatrix.back().size() + m_startNode.second;

        m_bfsData.m_visitedNodes.resize(nodesCount, false);
        m_bfsData.m_visitedNodes[startNodeIdx] = true;

        m_bfsData.m_visitQueue.push(m_startNode);
    }

    std::optional<NodePos_t> runBFSIteration() {
        while (!m_bfsData.m_visitQueue.empty()) {
            const auto& xPosition = m_bfsData.m_visitQueue.front();
            auto& xNode = (*this)[xPosition];
            if (xNode.getType() != Node::NodeType::CURRENTLY_ANALYZED) {
                xNode.setType(Node::NodeType::CURRENTLY_ANALYZED);

#ifndef GRAPH_DRAW_INSTANTLY
                return xPosition;
#endif
            }

            for (size_t i = 0; i < 4; ++i) {
                const NodePos_t yPosition = {xPosition.first + k_dirVec[i].first,
                                             xPosition.second + k_dirVec[i].second};
                if (isGoodPosition(yPosition)) {
                    size_t yFlattenedIdx =
                        yPosition.first * m_nodesMatrix.back().size() + yPosition.second;

                    if (!m_bfsData.m_visitedNodes[yFlattenedIdx]) {
                        m_bfsData.m_visitedNodes[yFlattenedIdx] = true;
                        m_bfsData.m_visitQueue.push(yPosition);

                        (*this)[yPosition].setType(Node::NodeType::VISITED);

#ifndef GRAPH_DRAW_INSTANTLY
                        return yPosition;
#endif
                    }
                }
            }

            xNode.setType(xPosition == m_startNode ? Node::NodeType::START
                                                   : Node::NodeType::ANALYZED);
            m_bfsData.m_visitQueue.pop();

#ifndef GRAPH_DRAW_INSTANTLY
            return xPosition;
#endif
        }

        return std::nullopt;
    }

    bool isGoodPosition(const NodePos_t& p) {
        return p.first < m_nodesMatrix.size() && p.second < m_nodesMatrix.back().size() &&
               (*this)[p].getType() == Node::NodeType::WALKABLE;
    }

   private:
    struct BFS {
        std::vector<bool> m_visitedNodes;
        std::queue<NodePos_t> m_visitQueue;
    } m_bfsData;

    std::vector<std::vector<Node>> m_nodesMatrix;
    NodePos_t m_startNode;

    static constexpr NodePos_t k_dirVec[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
};
