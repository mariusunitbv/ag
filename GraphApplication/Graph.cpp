#include "stdafx.h"
#include "Graph.h"

void Graph::AddNode(QPoint where) {
    Node* node = new Node(where, m_nodes.size());

    m_nodes.push_back(node);

    SaveGraph();
}

void Graph::ToggleSelectNode(Node* node) {
    if (node->IsSelected()) {
        node->Deselect();
        auto it =
            m_selectedNodes.erase(std::find(m_selectedNodes.begin(), m_selectedNodes.end(), node));
        for (; it != m_selectedNodes.end(); ++it) {
            Node* node = (*it);
            node->Select(node->GetSelectedIndex() - 1);
        }
    } else {
        node->Select(m_selectedNodes.size());
        m_selectedNodes.push_back(node);
    }
}

void Graph::SelectNodesAscending(size_t start, size_t end) {
    if (m_nodes.empty()) {
        return;
    }

    if (start >= m_nodes.size() || end > m_nodes.size()) {
        throw "Bad SelectNodesAscending() call";
    }

    for (size_t i = start; i < end; ++i) {
        ToggleSelectNode(m_nodes[i]);
    }
}

void Graph::SelectNodesDescending(size_t start, size_t end) {
    if (m_nodes.empty()) {
        return;
    }

    if (start >= m_nodes.size()) {
        throw "Bad SelectNodesDescending() call";
    }

    for (size_t i = start; i > end; --i) {
        ToggleSelectNode(m_nodes[i]);
    }

    ToggleSelectNode(m_nodes[end]);
}

void Graph::DeselectAllNodes() {
    for (auto node : m_selectedNodes) {
        node->Deselect();
    }

    m_selectedNodes.clear();
}

void Graph::SetMovedNode(Node* node) { m_movedNode = node; }

void Graph::ConnectNodes() {
    if (m_selectedNodes.size() < 2) {
        return;
    }

    for (size_t i = 0; i < m_selectedNodes.size() - 1; ++i) {
        AddEdge(m_selectedNodes[i], m_selectedNodes[i + 1]);
    }

    DeselectAllNodes();
}

void Graph::DeleteSelectedNodes() {
    if (m_selectedNodes.empty()) {
        return;
    }

    m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(),
                                 [&](const Edge& edge) {
                                     return edge.GetSource()->IsSelected() ||
                                            edge.GetTarget()->IsSelected();
                                 }),
                  m_edges.end());

    size_t lastIndexBeforeDeletion = 0;
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        Node* node = m_nodes[i];

        if (node->IsSelected()) {
        } else {
            node->SetIndex(lastIndexBeforeDeletion++);
        }
    }

    m_nodes.erase(std::remove_if(m_nodes.begin(), m_nodes.end(),
                                 [](Node* node) {
                                     if (node->IsSelected()) {
                                         delete node;
                                         return true;
                                     }

                                     return false;
                                 }),
                  m_nodes.end());

    DeselectAllNodes();
    SaveGraph();
}

void Graph::AddEdge(Node* l, Node* r) {
    auto it = std::find_if(m_edges.begin(), m_edges.end(), [&](const Edge& edge) {
        return edge.GetSource() == l && edge.GetTarget() == r;
    });

    if (it != m_edges.end()) {
        return;
    }

    m_edges.emplace_back(l, r);

    SaveGraph();
}

void Graph::TerminateEdgesOfSelectedNodes() {
    if (m_selectedNodes.size() < 2) {
        return;
    }

    for (size_t i = 0; i < m_selectedNodes.size() - 1; ++i) {
        Node *source = m_selectedNodes[i], *target = m_selectedNodes[i + 1];
        m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(),
                                     [&](const Edge& edge) {
                                         return edge.GetSource() == source &&
                                                edge.GetTarget() == target;
                                     }),
                      m_edges.end());
    }

    SaveGraph();
}

Node* Graph::GetNearestNode(QPoint where, int additionalRadius) {
    for (auto node : m_nodes) {
        QPoint delta = node->GetCenter() - where;
        if (abs(delta.x()) <= Node::kNodeRadius + additionalRadius &&
            abs(delta.y()) <= Node::kNodeRadius + additionalRadius) {
            return node;
        }
    }

    return nullptr;
}

const Node* Graph::GetNearestNode(QPoint where, int additionalRadius) const {
    for (auto node : m_nodes) {
        QPoint delta = node->GetCenter() - where;
        if (abs(delta.x()) <= Node::kNodeRadius + additionalRadius &&
            abs(delta.y()) <= Node::kNodeRadius + additionalRadius) {
            return node;
        }
    }

    return nullptr;
}

bool Graph::CanPlaceNode(QPoint where) const {
    if (where.x() <= Node::kNodeRadius || where.y() <= Node::kNodeRadius) {
        return false;
    }

    return GetNearestNode(where, Node::kNodeRadius) == nullptr;
}

bool Graph::CanMoveNode(Node* movedNode, QPoint where) const {
    if (where.x() <= Node::kNodeRadius || where.y() <= Node::kNodeRadius) {
        return false;
    }

    for (auto node : m_nodes) {
        if (node == movedNode) {
            continue;
        }

        QPoint delta = node->GetCenter() - where;
        if (abs(delta.x()) <= Node::kNodeRadius * 2 && abs(delta.y()) <= Node::kNodeRadius * 2) {
            return false;
        }
    }

    return true;
}

const std::vector<Node*>& Graph::GetNodes() const { return m_nodes; }

const std::vector<Edge>& Graph::GetEdges() const { return m_edges; }

Node* Graph::GetMovedNode() const { return m_movedNode; }

void Graph::SaveGraph() const {
    std::ofstream fin{"graph.out"};

    const size_t nodesCount = m_nodes.size();

    fin << nodesCount << '\n';

    std::vector<bool> adjacency;
    adjacency.resize(nodesCount * nodesCount);
    for (const auto& edge : m_edges) {
        size_t srcIdx = edge.GetSource()->GetIndex(), targetIdx = edge.GetTarget()->GetIndex();
        adjacency[srcIdx * nodesCount + targetIdx] = true;
    }

    for (size_t i = 0; i < nodesCount; ++i) {
        for (size_t j = 0; j < nodesCount; ++j) {
            fin << adjacency[i * nodesCount + j] << ' ';
        }
        fin << '\n';
    }
}
