#pragma once

class Node {
   public:
    Node(QPoint pos = {}, size_t index = 0);

    size_t GetIndex() const;
    size_t GetSelectedIndex() const;

    QPoint GetCenter() const;
    QRect GetRectangle() const;
    QRect GetSelectedIndexDrawPosition() const;

    void SetPosition(QPoint pos);
    void SetIndex(size_t index);
    void Select(size_t selectedIndex);
    void Deselect();
    bool IsSelected() const;

    static const int kNodeRadius;
    static constexpr size_t kUnselectedIndex = static_cast<size_t>(-1);

   private:
    size_t m_index, m_selectedIndex;
    QPoint m_pos;
};
