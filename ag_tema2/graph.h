#pragma once
#include <queue>

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
    ~GridGraph() {}

    Node* operator[](const std::pair<size_t, size_t>& p) {
        return m_nodesMatrix[p.first][p.second];
    }

    void addNode(Node::NodeType nodeType) {
        auto& lastRow = m_nodesMatrix.back();

        lastRow.push_back(nodeType == Node::NodeType::WALL ? nullptr : new Node{nodeType});
        if (nodeType == Node::NodeType::START) {
            m_startNode = std::make_pair(m_nodesMatrix.size() - 1, lastRow.size() - 1);
        }
    }

    void addRow() { m_nodesMatrix.emplace_back(); }

    const auto& getStartNodeIndex() const { return m_startNode; }

    auto& getGrid() { return m_nodesMatrix; }

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

    bool runBFSIteration() {
        while (!m_bfsData.m_visitQueue.empty()) {
            const auto& xPosition = m_bfsData.m_visitQueue.front();
            (*this)[xPosition]->setType(Node::NodeType::CURRENTLY_ANALYZED);

            for (size_t i = 0; i < 4; ++i) {
                const decltype(m_startNode) yPosition = {xPosition.first + k_dirVec[i].first,
                                                         xPosition.second + k_dirVec[i].second};
                if (isGoodPosition(yPosition)) {
                    size_t yFlattenedIdx =
                        yPosition.first * m_nodesMatrix.back().size() + yPosition.second;

                    if (!m_bfsData.m_visitedNodes[yFlattenedIdx]) {
                        m_bfsData.m_visitedNodes[yFlattenedIdx] = true;
                        m_bfsData.m_visitQueue.push(yPosition);

                        (*this)[yPosition]->setType(Node::NodeType::VISITED);
                        return true;
                    }
                }
            }

            (*this)[xPosition]->setType(Node::NodeType::ANALYZED);
            m_bfsData.m_visitQueue.pop();
            return true;
        }

        return false;
    }

    bool isGoodPosition(const std::pair<size_t, size_t>& p) {
        if (0 <= p.first && p.first < m_nodesMatrix.size()) {
            if (0 <= p.second && p.second < m_nodesMatrix.back().size()) {
                Node* node = (*this)[p];
                if (node && node->getType() == Node::NodeType::WALKABLE) {
                    return true;
                }
            }
        }

        return false;
    }

   private:
    struct BFS {
        std::vector<bool> m_visitedNodes;
        std::queue<std::pair<size_t, size_t>> m_visitQueue;
    } m_bfsData;

    std::vector<std::vector<Node*>> m_nodesMatrix;
    std::pair<size_t, size_t> m_startNode;

    static constexpr std::pair<size_t, size_t> k_dirVec[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
};
