#pragma once

#include "Edge.h"

class Graph {
   public:
    void AddNode(QPoint where);

    void ToggleSelectNode(Node* node);
    void SelectNodesAscending(size_t start, size_t end);
    void SelectNodesDescending(size_t start, size_t end);
    void DeselectAllNodes();

    std::vector<std::pair<int, std::vector<Node*>>> FindAllShortestPaths();

    void SetDraggedNode(Node* node);
    void DragNode(Node* node, QPoint where);

    void ConnectNodes();
    void DeleteSelectedNodes();

    void AddEdge(Node* l, Node* r, int cost);
    void TerminateEdgesOfSelectedNodes();

    Node* GetNearestNode(QPoint where, int additionalRadius = 0);
    const Node* GetNearestNode(QPoint where, int additionalRadius = 0) const;

    bool CanPlaceNode(QPoint where) const;
    bool CanDragNodeTo(Node* movedNode, QPoint where) const;

    const std::vector<Node*>& GetNodes() const;
    const std::vector<Edge>& GetEdges() const;
    Node* GetDraggedNode() const;

    static constexpr int kEdgeArrowSize = 20;

    const std::string& getTopologicalOrderDebugString() const {
        return topologicalOrderDebugString;
    }

   private:
    std::vector<Node*> m_nodes, m_selectedNodes;
    std::vector<Edge> m_edges;

    std::string topologicalOrderDebugString;

    Node* m_draggedNode;
};
