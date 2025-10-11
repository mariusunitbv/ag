#include "stdafx.h"
#include "Edge.h"

Edge::Edge(Node* l, Node* r) : m_pair{std::make_pair(l, r)} {}

Node* Edge::GetSource() const { return m_pair.first; }

Node* Edge::GetTarget() const { return m_pair.second; }

bool Edge::IsSelected() const {
    return GetSource()->IsSelected() && GetTarget()->IsSelected() &&
           GetSource()->GetSelectedIndex() < GetTarget()->GetSelectedIndex();
}
