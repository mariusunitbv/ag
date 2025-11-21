#pragma once

#include "Node.h"

class Edge {
   public:
    Edge(Node* l, Node* r, int cost);

    Node* GetSource() const;
    Node* GetTarget() const;

    bool IsSelected() const;

    bool SameAs(const Edge& rhs) const;
    bool SameAs(Node* l, Node* r) const;

    int GetCost() const;

   private:
    std::pair<Node*, Node*> m_pair;
    int m_cost;
};
