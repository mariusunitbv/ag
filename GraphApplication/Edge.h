#pragma once

#include "Node.h"

class Edge {
   public:
    Edge(Node* l, Node* r);

    Node* GetSource() const;
    Node* GetTarget() const;

    bool IsSelected() const;

    bool SameAs(const Edge& rhs) const;
    bool SameAs(Node* l, Node* r) const;
    bool SameAsUnoriented(const Edge& rhs) const;
    bool SameAsUnoriented(Node* l, Node* r) const;

   private:
    std::pair<Node*, Node*> m_pair;
};
