#include "stdafx.h"
#include "Edge.h"

Edge::Edge(Node* l, Node* r, int cost) : m_cost{cost}, m_pair{std::make_pair(l, r)} {}

Node* Edge::GetSource() const { return m_pair.first; }

Node* Edge::GetTarget() const { return m_pair.second; }

bool Edge::IsSelected() const {
    return GetSource()->IsSelected() && GetTarget()->IsSelected() &&
           (GetSource()->GetSelectedIndex() == GetTarget()->GetSelectedIndex() - 1);
}

bool Edge::SameAs(const Edge& rhs) const {
    return GetSource() == rhs.GetSource() && GetTarget() == rhs.GetTarget();
}

bool Edge::SameAs(Node* l, Node* r) const { return GetSource() == l && GetTarget() == r; }

int Edge::GetCost() const { return m_cost; }
