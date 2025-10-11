#include "stdafx.h"
#include "Node.h"

const int Node::kNodeRadius = 23;

Node::Node(QPoint pos, size_t index)
    : m_pos{pos}, m_index{index}, m_selectedIndex{kUnselectedIndex} {}

size_t Node::GetIndex() const { return m_index; }

size_t Node::GetSelectedIndex() const { return m_selectedIndex; }

QPoint Node::GetCenter() const { return m_pos; }

QRect Node::GetRectangle() const {
    return QRect{m_pos.x() - kNodeRadius, m_pos.y() - kNodeRadius, kNodeRadius * 2,
                 kNodeRadius * 2};
}

QRect Node::GetSelectedIndexDrawPosition() const {
    return QRect{m_pos.x() - kNodeRadius, m_pos.y() - kNodeRadius + 13, kNodeRadius * 2,
                 kNodeRadius * 2};
}

void Node::SetPosition(QPoint pos) { m_pos = pos; }

void Node::SetIndex(size_t index) { m_index = index; }

void Node::Select(size_t selectedIndex) { m_selectedIndex = selectedIndex; }

void Node::Deselect() { m_selectedIndex = kUnselectedIndex; }

bool Node::IsSelected() const { return m_selectedIndex != kUnselectedIndex; }
