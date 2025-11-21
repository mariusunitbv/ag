#include "stdafx.h"
#include "Graph.h"

void Graph::AddNode(QPoint where) {
    Node* node = new Node(where, m_nodes.size());

    m_nodes.push_back(node);
}

void Graph::ToggleSelectNode(Node* node) {
    if (node->IsSelected()) {
        auto it = m_selectedNodes.erase(m_selectedNodes.begin() + node->GetSelectedIndex());
        node->Deselect();
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

std::vector<std::pair<int, std::vector<Node*>>> Graph::FindAllShortestPaths() {
    if (m_selectedNodes.size() != 1) {
        QMessageBox::warning(nullptr, "Warning", "Un singur nod trebuie selectat.");
        return {};
    }

    topologicalOrderDebugString = "Ordine topologica: ";

    size_t n = m_nodes.size();

    std::vector<bool> visited(n, false);
    std::vector<bool> onStack(n, false);
    std::stack<Node*> visitedStack;

    std::vector<std::vector<Edge>> adjacencyList(n);
    std::vector<Node*> topologicalOrder;

    for (const auto& edge : m_edges) {
        int u = edge.GetSource()->GetIndex();
        adjacencyList[u].push_back(edge);
    }

    visited[m_selectedNodes[0]->GetIndex()] = true;
    onStack[m_selectedNodes[0]->GetIndex()] = true;
    visitedStack.push(m_selectedNodes[0]);

    while (!visitedStack.empty()) {
        const auto x = visitedStack.top();

        bool foundUnvisited = false;
        for (const auto& edge : adjacencyList[x->GetIndex()]) {
            const auto y = edge.GetTarget();

            if (onStack[y->GetIndex()]) {
                QMessageBox::warning(nullptr, "Nu se poate realiza algoritmul",
                                     "Graful are cicluri");
                return {};
            }

            if (!visited[y->GetIndex()]) {
                onStack[y->GetIndex()] = true;

                visited[y->GetIndex()] = true;
                visitedStack.push(y);
                foundUnvisited = true;
                break;
            }
        }

        if (!foundUnvisited) {
            onStack[x->GetIndex()] = false;

            topologicalOrder.push_back(x);
            visitedStack.pop();
        }
    }

    std::reverse(topologicalOrder.begin(), topologicalOrder.end());
    for (const auto node : topologicalOrder) {
        topologicalOrderDebugString += std::to_string(node->GetIndex() + 1) + " ";
    }

    std::vector<int> d(n, INT_MAX);
    std::vector<Node*> p(n, nullptr);

    d[m_selectedNodes[0]->GetIndex()] = 0;
    auto startIt = std::find(topologicalOrder.begin(), topologicalOrder.end(), m_selectedNodes[0]);

    for (auto it = topologicalOrder.begin(); it != topologicalOrder.end(); ++it) {
        int iIndex = (*it)->GetIndex();
        if (d[iIndex] == INT_MAX) {
            continue;
        }

        for (auto& edge : adjacencyList[iIndex]) {
            int jIndex = edge.GetTarget()->GetIndex();

            if (d[iIndex] + edge.GetCost() < d[jIndex]) {
                d[jIndex] = d[iIndex] + edge.GetCost();
                p[jIndex] = *it;
            }
        }
    }

    std::vector<std::pair<int, std::vector<Node*>>> allPaths;
    for (size_t vIndex = 0; vIndex < n; ++vIndex) {
        if (d[vIndex] == INT_MAX || vIndex == m_selectedNodes[0]->GetIndex()) continue;

        std::vector<Node*> path;
        for (int at = vIndex; at != -1; at = p[at] ? p[at]->GetIndex() : -1) {
            path.push_back(m_nodes[at]);
        }

        std::reverse(path.begin(), path.end());
        allPaths.push_back(std::make_pair(vIndex, path));
    }

    return allPaths;
}

void Graph::SetDraggedNode(Node* node) { m_draggedNode = node; }

void Graph::DragNode(Node* node, QPoint where) { node->SetPosition(where); }

void Graph::ConnectNodes() {
    if (m_selectedNodes.size() < 2) {
        return;
    }

    for (size_t i = 0; i < m_selectedNodes.size() - 1; ++i) {
        bool ok;
        int value = QInputDialog::getInt(nullptr, "Enter cost",
                                         QString("Cost from %1 to %2")
                                             .arg(m_selectedNodes[i]->GetIndex() + 1)
                                             .arg(m_selectedNodes[i + 1]->GetIndex() + 1),
                                         0, 0, 100, 1, &ok);
        if (!ok) {
            value = 0;
        }

        AddEdge(m_selectedNodes[i], m_selectedNodes[i + 1], value);
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

    m_selectedNodes.clear();
}

void Graph::AddEdge(Node* l, Node* r, int cost) {
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
                           [&](const Edge& edge) { return edge.SameAs(l, r); });

    if (it != m_edges.end()) {
        return;
    }

    m_edges.emplace_back(l, r, cost);
}

void Graph::TerminateEdgesOfSelectedNodes() {
    if (m_selectedNodes.size() < 2) {
        return;
    }

    for (size_t i = 0; i < m_selectedNodes.size() - 1; ++i) {
        Node *source = m_selectedNodes[i], *target = m_selectedNodes[i + 1];
        m_edges.erase(std::remove_if(m_edges.begin(), m_edges.end(),
                                     [&](const Edge& edge) { return edge.SameAs(source, target); }),
                      m_edges.end());
    }
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

bool Graph::CanDragNodeTo(Node* movedNode, QPoint where) const {
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

Node* Graph::GetDraggedNode() const { return m_draggedNode; }
