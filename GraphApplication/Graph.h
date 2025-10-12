#pragma once

#include "Edge.h"

class Graph {
   public:
    void AddNode(QPoint where);

    void ToggleSelectNode(Node* node);
    void SelectNodesAscending(size_t start, size_t end);
    void SelectNodesDescending(size_t start, size_t end);
    void DeselectAllNodes();

    void SetDraggedNode(Node* node);
    void DragNode(Node* node, QPoint where);

    void ConnectNodes();
    void DeleteSelectedNodes();

    void AddEdge(Node* l, Node* r);
    void TerminateEdgesOfSelectedNodes();

    Node* GetNearestNode(QPoint where, int additionalRadius = 0);
    const Node* GetNearestNode(QPoint where, int additionalRadius = 0) const;

    bool CanPlaceNode(QPoint where) const;
    bool CanDragNodeTo(Node* movedNode, QPoint where) const;

    const std::vector<Node*>& GetNodes() const;
    const std::vector<Edge>& GetEdges() const;
    Node* GetDraggedNode() const;

    void MakeGraphUnoriented();
    void SetOrientedGraph(bool state);
    bool IsOrientedGraph() const;

    void SaveGraph() const;

    static constexpr int kEdgeArrowSize = 20;

   private:
    std::vector<Node*> m_nodes, m_selectedNodes;
    std::vector<Edge> m_edges;

    Node* m_draggedNode;
    bool m_unorientedGraph;
};
