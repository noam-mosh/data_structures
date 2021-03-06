#ifndef WET2_NODE_H
#define WET2_NODE_H
#include <stdio.h>
#include <stdlib.h>

namespace data_structures {
template<class T>
class Node
{
    public:
    Node() = default;
    Node(T element):element(element),next(nullptr){};
    Node(T element,Node* ptr):element(element),next(ptr){};
    ~Node()=default;
    //Node(const Node& other)=default;
    Node& operator=(const Node<T>& node) = default;
    
    //private:
    T element;
    Node* next;
};
}
#endif //WET2_NODE_H