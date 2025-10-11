#pragma once

#include "Edge.h"

class Graph {
   public:
    void AddNode(QPoint where);

    void ToggleSelectNode(Node* node);
    void SelectNodesAscending(size_t start, size_t end);
    void SelectNodesDescending(size_t start, size_t end);
    void DeselectAllNodes();

    void SetMovedNode(Node* node);

    void ConnectNodes();
    void DeleteSelectedNodes();

    void AddEdge(Node* l, Node* r);
    void TerminateEdgesOfSelectedNodes();

    Node* GetNearestNode(QPoint where, int additionalRadius = 0);
    const Node* GetNearestNode(QPoint where, int additionalRadius = 0) const;

    bool CanPlaceNode(QPoint where) const;
    bool CanMoveNode(Node* movedNode, QPoint where) const;

    const std::vector<Node*>& GetNodes() const;
    const std::vector<Edge>& GetEdges() const;
    Node* GetMovedNode() const;

    void SaveGraph() const;

    static constexpr int kEdgeArrowSize = 20;

   private:
    std::vector<Node*> m_nodes, m_selectedNodes;
    std::vector<Edge> m_edges;

    Node* m_movedNode;
};
