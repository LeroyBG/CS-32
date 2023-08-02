#include "Sequence.h"
#include <iostream>

using namespace std;
// first we're gonna define the node struct
Sequence::Node::Node(){
    m_next = this;
    m_prev = this;
}

Sequence::Sequence(){
    m_head = new Node;
    if (size() != 0)
        cerr << "ERROR: sequence initialization";
}

Sequence& Sequence::operator=(Sequence &rhs){
    Sequence swapme = rhs;
    swap(swapme);
    return (*this);
}

Sequence::Sequence(const Sequence &other){
    // we're making a new sequence that's basically identical to the old one, so we've got to make a loop that creates new nodes corresponding to the other sequence's nodes
    m_head = new Node; // we've got our dummy node
    if (size() != 0)
        cerr << "ERROR: copy construction!" << endl;
    // now we need to loop through the other sequence, grabbing its nodes' values until the address points back to its head node
    // start at the other list's 0 node, not their dummy node
    
    if (other.size() == 0){
        if (other.lastNode() != (-1)){
            cerr << "Size-node conversion error! (copy construction)" << endl;
        }
        else{
            cerr << "Copy construction done! List only contains dummy node" << endl;
        }
    }
    for (int i = 0; other.IndAddconv(i) != nullptr; i++){ // when the converted address equals nullptr, we know we've reached the end of other list
        Node* otherNode = other.IndAddconv(i); // we start at other's 0 node
        ItemType otherNodeVal = otherNode->m_data;
        newNode(otherNodeVal, m_head->m_prev, m_head); // creates a new node with previous: the former last element, next: head, and reassigns those nodes to point to it
        
    }
}

Sequence::Node* Sequence::IndAddconv(int pos) const{ // returns the memory address of the node at the specified position, nullptr if that node doesn't exist (not always a bad thing)
    
    Node* address = nullptr; // will hold the address we find
    Node* p = m_head;
    int i = (-1); // index will increment towards pos until they're equal
    do {
        if (i == pos){
            address = p;
            break;
        }
        p = p->m_next;
        i++;
    } while (p != m_head); // last node it checks is the last one, the one pointing to head
    
    return address;
}

int Sequence::lastNode() const{
    Node* p = m_head;
    int lastnode = -1; // we start at 0 because we skip the dummy node
    while (p->m_next != m_head){ // loop iterates until it reaches back to the front of the list, counting the elements
        p = p->m_next;
        lastnode++;
    }
    return lastnode;
}

Sequence::Node* Sequence::newNode(ItemType const &value, Node* previous, Node* next) const{
    if (previous == nullptr || next == nullptr){
        cerr << "something's very wrong (newNode)" << endl;
        return nullptr;
    }
    Node* holdNodeptr = new Node;
    
    holdNodeptr->m_data = value; // give the new node the specified data and addresss
    holdNodeptr->m_prev = previous;
    holdNodeptr->m_next = next;
        
    previous->m_next = holdNodeptr; // make previous and next nodes point to this
    next->m_prev = holdNodeptr;
    
    return holdNodeptr;
}

bool Sequence::empty() const{
    // an empty sequence only contains the dummy node pointing to itself, so if the previous and next node both point to the address of the node itself, the list is empty
    if (m_head->m_next == m_head && m_head->m_prev == m_head){ // head->next is the data member pointing to the 'next' element, converse true with ->prev
        return true;
    }
    return false;
}

int Sequence::insert(int pos, const ItemType &value){
    // position 0 means the SECOND node
    // should NEVER return negative one because it found the index of the dummy node, but can return negative one if the node doesn't exist
    
    Node* moveNode = IndAddconv(pos); // stores the address of the node at the specified position
    
    if (moveNode == nullptr){ // if that node doesn't exist
        // either that node is out of bounds or we're trying to add to the end of the list
        if (pos == size()){
            // create the node
            Node* before = m_head->m_prev;
            Node* after = m_head;
            newNode(value, before, after);
            /* creates a new node and hooks it up to the list, very proud of myself for this code */
            return pos;
        }
        return (-1);
    }
    
    // otherwise we know there's a node at the specified position, so we can create a node and link it to our list
    
    // we have the address of the next node (the one we want to attach to)
    
    Node* before = moveNode->m_prev; // get pointers to adjacent nodes
    Node* after = moveNode;
    
    Node* nodeHolder = newNode(value, before, after);
    if (nodeHolder == nullptr){
        cerr << "ERROR! (insert)" << endl;
    }
    return pos;
}


int Sequence::size() const{
    return (lastNode() + 1);
    /* while it's more efficent to store a variable tracking list size, list's allow you to count their size, so I'd rather take the performance hit than risk forgetting to incremement/decrement size once and hurting the rest of the program */
}


int Sequence::insert(const ItemType& value){
    Node* p = m_head->m_next;
    int i = 0; // index will increment towards pos until they're equal
    while (p != m_head){
        if (value <= p->m_data){
            break;
        }
        p = p->m_next;
        i++;
    }
    
    int returnval = insert(i, value);
    if (returnval == (-1)){
        cerr << "ERROR: item not inserted (insert w/one parameter)" << endl;
    }
    
    return returnval;
}

bool Sequence::erase(int pos){
    if (pos < 0 || pos > lastNode()){
        return false;
    }
    Node* n = IndAddconv(pos);
    if (n == nullptr)
        return false;
    
    // the node n points to will be deleted
    Node* before_n = n->m_prev;
    Node* after_n = n->m_next;
    
    // the node before n should point to the one after it
    before_n->m_next = after_n;
    
    // and the one after it should point to the one before it
    after_n->m_prev = before_n;
    
    delete n; // delete the node
    
    return true;
}

int Sequence::remove(const ItemType& value){
    Node* p = m_head->m_next;
    int i = 0; // index will increment towards pos until they're equal
    int removed = 0;
    while (p != m_head){
        if (value == p->m_data){
            p = p->m_next;
            erase(i);
            removed++;
            continue;
        }
        p = p->m_next;
        i++;
    }
        

    return removed;
}


bool Sequence::get(int pos, ItemType& value) const{
    Node* n = IndAddconv(pos); // store address of specifed item
    if (n == nullptr){ // if that item doesn't exist
        return false;
    }
    
    value = n->m_data; // give value the data stored in the specifed node
    return true;
}

bool Sequence::set(int pos, const ItemType& value){
    Node* n = IndAddconv(pos); // store address of specifed item
    if (n == nullptr){ // if that item doesn't exist
        return false;
    }
    
    n->m_data = value; // give the node the data specified in value
    return true;
}

int Sequence::find(const ItemType& value) const{
    Node* p = m_head->m_next;
    int i = 0; // index will increment towards pos until they're equal
    while (p != m_head){
        if (value == p->m_data){
            return i;
        }
        p = p->m_next;
        i++;
    }
    return (-1); // we didn't find the item
}

void Sequence::swap(Sequence& other){
    Node* myOldHead = m_head; // store current head
    m_head = other.m_head; // give current head other's head
    other.m_head = myOldHead; // give other our old head
}

Sequence::~Sequence(){ // iterate through list destroying each node
    // set the "last" element to point to null, just to make things easier to understand
    cerr << "deleting list of size " << size() << " elements. should delete " << (size() + 1) << " elements." << endl;
    int deleted = 0;
    Node* last = m_head->m_prev;
    
    if (last->m_next != m_head)
        cerr << "ERROR: last node should point to head!" << endl;
    
    last->m_next = nullptr;
    Node* p = m_head;
    while (p != nullptr){
        Node* eraseMe = p;
        p = p->m_next; // get the address of the next node
        delete eraseMe;
        deleted++;
    }
    cerr << "deleted " << deleted << " elements. Destructor finished." << endl;
}

int subsequence(const Sequence& seq1, const Sequence& seq2){
    if (seq1.size() == 0 || seq2.size() == 0){ // edge cases loop can't handle
        return (-1); // this is the case we're they're the same sequence but have size greater than 0
    }
    if (&seq1 == &seq2)
        return 0;
    
    ItemType seq1val, seq2val;
    // loop that goes through the list looking for items
    for (int i = 0; i < seq1.size(); i++){
        if (seq1.get(i, seq1val) && seq2.get(0, seq2val)){ // if we can successfully retrieve the values at seq1 and seq2 at the given index
            // so now we know that the current element in seq1 is the same as the first in seq2
            if (seq1val == seq2val){ // let's check to see if the rest are the same too (add a break when they aren't the same
                for (int g = 0; g <= seq2.size(); g++){
                    
                    if (g == seq2.size()){
                        return i; // if we reached the end of sequence 2, return the index of where we started this loop
                    }
                    if (!seq1.get((i+g), seq1val) || !seq2.get(g, seq2val)){ // i represents the first item in seq that corresponds to the first element of seq2
                        break; // if we can't access one of these items
                    }
                    if (seq1val != seq2val){ // if we reach a part where the sequences aren't the same
                        break;
                    }
                    
                }
            }
        }
    }
    return (-1);
}

void concatReverse(const Sequence& seq1, const Sequence& seq2, Sequence& result){
    Sequence seq1_copy = seq1, seq2_copy = seq2; // create copies of sequences 1 and 2 for aliasing resistance
    
    // delete all nodes in result other than the head node
    for (int i = (result.size() - 1); i >= 0; i--){
        result.erase(i);
    }
    if (result.size() != 0){
        cerr << "ERROR: result not emptied correctly. (concatReverse)" << endl;
    }
    // now all we have left of result is the head node
    
    // reverse and add seq1 to result
    int g = 0; // counter starts at 0 so we can index seq1 from the beginning
    if (seq1_copy.size() > 0){
        for (int i = (seq1_copy.size() - 1); i >= 0; i--){ // start at the last element of sequence 1 and decrement
            ItemType temp;
            seq1_copy.get(i, temp); // start at the end
            result.insert(g, temp); // start at the beginning
            g++;
        }
    }
    // reverse and add seq2 to result
    if (seq2_copy.size() > 0){
        for (int i = (seq2_copy.size() - 1); i >= 0; i--){ // start at the last element of sequence 1 and decrement
            ItemType temp;
            seq2_copy.get(i, temp); // start at the end
            result.insert(g, temp); // start at the beginning
            g++;
        }
    }
    if (result.size() != (seq1_copy.size() + seq2_copy.size())){
        cerr << "ERROR: result sequence should have combined size of seq1 and se12. (concatreverse)" << endl;
    }
}

void Sequence::dump() const{
    cerr << "dumping->|";
    for (int i = 0; i < size(); i++){
        ItemType temp;
        get(i, temp);
        cerr << temp;
    }
    cerr << "|<-done." << endl;
}
