#pragma once

#include "Node.h"

class Edge {
   public:
    Edge(Node* l, Node* r);

    Node* GetSource() const;
    Node* GetTarget() const;

    bool IsSelected() const;

   private:
    std::pair<Node*, Node*> m_pair;
};
