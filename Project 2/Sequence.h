#ifndef SEQUENCE_H
#define SEQUENCE_H
#include <string>
using ItemType = std::string;


class Sequence
{
  public:
    Sequence();
    Sequence(const Sequence &other);
    bool empty() const;
    int size() const;
    int insert(int pos, const ItemType& value);
    int insert(const ItemType& value);
    bool erase(int pos);
    int remove(const ItemType& value);
    bool get(int pos, ItemType& value) const;
    bool set(int pos, const ItemType& value);
    int find(const ItemType& value) const;
    void swap(Sequence& other);
    ~Sequence();
    Sequence& operator=(Sequence &rhs);
    void dump() const;
    
    // my data members
    
private:
    struct Node{
        Node();
        ItemType m_data;
        Node* m_next;
        Node* m_prev;
    };
    // data member(s)
    Node* m_head;

    // functions
    int lastNode() const;
    Node* newNode(ItemType const &value, Node* previous, Node* next) const;
    Node* IndAddconv(int pos) const;
};

int subsequence(const Sequence& seq1, const Sequence& seq2);
void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result);

#endif // SEQUENCE_H defined
