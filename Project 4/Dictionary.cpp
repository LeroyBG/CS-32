// Dictionary.cpp

#include "Dictionary.h"
#include <string>
#include <list>
#include <cctype>
#include <utility>  // for swap
#include <functional> // for hash function
#include <cstdlib> // DELETE: for scramble
using namespace std;

void removeNonLetters(string& s);
void generateNextPermutation(string& permutation);

  // This class does the real work of the implementation.


// ok we've got 3.5 options. I have no idea how efficient each one would be
    // we just use maxbuckets as the number of buckets
    // 1 we find the greatest prime number <= maxbuckets when the dictionary is initialized (to avoid collisions). This might cause a big performance hit when you initialize a large bucket
    // we pre-store all (5133) prime numbers between 1 and 50,000 in a list and do a binary search for the number less than this one
        // ok i don't know how I'd do this
    // a combination of the two
    
class DictionaryImpl
{
  public:
    DictionaryImpl(int maxBuckets)
    : m_maxBuckets(maxBuckets) {
        hashtest();
        m_words.assign(maxBuckets, nullptr);}
    ~DictionaryImpl(){
        for (int i = 0; i < m_words.size(); i++){
            delete m_words[i];
        }
    }
    void insert(string word);
    void lookup(string letters, void callback(string)) const;
  private:
    // ideal load factor: 0.7? idk
    
    unsigned long hasher(string word, string &sortMe) const;
    struct Node{
        string val;
        string sorted;
        Node* next;
        
        Node(string value = string(), string sorted_value = string(), Node* nextNode = nullptr)
        : val(value), next(nextNode) , sorted(sorted_value){}
        ~Node();
    };
    int m_maxBuckets;
    vector<Node*> m_words;
    
    /* DELETE ---- test functions */
    void hashtest() const;
    string scramble(string word) const;
};

unsigned long DictionaryImpl::hasher(string word, string &sortMe) const{
    sortMe = word;
    sort(sortMe.begin(), sortMe.end()); // sort the word
    // all anagrams look the same after you sort them
    // because hashing is deterministic and the fact above
        // all anagrams will have the same hash value after this
    
    unsigned long hashnum = hash<string>()(sortMe);
    return hashnum % m_maxBuckets; // we're going to get more collisions
    
    
}
void DictionaryImpl::insert(string word)
{
    removeNonLetters(word);
    if (word.empty())
        return;
    
    string sorted; // will be sorted by our hash function
    unsigned long index = hasher(word, sorted);
    
    Node* prevptr = m_words[index];
    m_words[index] = new Node(word, sorted, prevptr); /* assign the bucket to point to a new node, and that new node points to whatever the bucket pointed to beforehand: if there was already a list there, appends to front, if there wasn't a node there, a new one is created */
    
}

void DictionaryImpl::lookup(string letters, void callback(string)) const
{
    if (callback == nullptr)
        return;

    removeNonLetters(letters);
    if (letters.empty())
        return;
    
    string this_sorted;
    unsigned long index = hasher(letters, this_sorted);
    
    Node* ptr = m_words[index];
    
    while (ptr){ // loop through linked list, looking for anagrams
        if (this_sorted == ptr->sorted)
            callback(ptr->val);
        ptr = ptr->next;
    }
    
}

void removeNonLetters(string& s)
{
    string::iterator to = s.begin();
    for (string::const_iterator from = s.begin(); from != s.end(); from++)
    {
        if (isalpha(*from))
        {
            *to = tolower(*from);
            to++;
        }
    }
    s.erase(to, s.end());  // chop everything off from "to" to end.
}

  // Each successive call to this function generates the next permutation of the
  // characters in the parameter.  For example,
  //    string s = "eel";
  //    generateNextPermutation(s);  // now s == "ele"
  //    generateNextPermutation(s);  // now s == "lee"
  //    generateNextPermutation(s);  // now s == "eel"
  // You don't have to understand this function's implementation.
void generateNextPermutation(string& permutation)
{
    string::iterator last = permutation.end() - 1;
    string::iterator p;

    for (p = last; p != permutation.begin()  &&  *p <= *(p-1); p--)
        ;
    if (p != permutation.begin())
    {
        string::iterator q;
        for (q = p+1; q <= last  &&  *q > *(p-1); q++)
            ;
        swap(*(p-1), *(q-1));
    }
    for ( ; p < last; p++, last--)
        swap(*p, *last);
}

DictionaryImpl::Node::~Node(){
    delete this->next;
}

string DictionaryImpl::scramble(string word) const{
    string scrambled;
    srand( int(time(0)) );
    
    if (word.empty())
        return "";
    
    while (!word.empty()){
        int index = rand() % word.size();
        scrambled += word[index];
        word.erase(index, 1);
    }
    return scrambled;
}

void DictionaryImpl::hashtest() const{
    string str1, str2;
    string sort1, sort2;
    
    // test anagrams
    str1 = "hello";
    str2 = "ohell";
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);
    
    str1 = "looted";
    str2 = "toledo";
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "lakjfdsln";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "oijsdfjkndf";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "asfgw";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "assdfsdg";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "s";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "dsggsd";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    str1 = "oijsasasdfjkndf";
    str2 = scramble(str1);
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

    
    // test number dropping
    str1 = "hello1";
    removeNonLetters(str1);
    str2 = "hello";
    assert(hasher(str1, sort1) == hasher(str2, sort2));
    assert(sort1 == sort2);

}

//******************** Dictionary functions ******************************

// These functions simply delegate to DictionaryImpl's functions
// You probably don't want to change any of this code

Dictionary::Dictionary(int maxBuckets)
{
    m_impl = new DictionaryImpl(maxBuckets);
}

Dictionary::~Dictionary()
{
    delete m_impl;
}

void Dictionary::insert(string word)
{
    m_impl->insert(word);
}

void Dictionary::lookup(string letters, void callback(string)) const
{
    m_impl->lookup(letters,callback);
}
