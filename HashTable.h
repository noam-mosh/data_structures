
#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include "Node.h"

template<class T>
class HashTable{
    public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable& other)=default;//check
    HashTable& operator=(const HashTable& other)=default;//check
    void Insert(T element);
    Node<T>* Find(T element);//returns the Node with the wanted data
    void remove(T element);//assuming the data exist
    //int getSize();

    //private:
    int elements_num;
    int table_size;
    Node<T>** table;
    void Resize();
    int HashFunction(int key);
};

template<class T>
HashTable<T>::HashTable():elements_num(0),table_size(P),table(new Node<T>*[P]{nullptr}){
    //what to do with alloc errors?
}

template<class T>
HashTable<T>:: ~HashTable(){
    for(int i=0; i<table_size;i++){
        if(table[i]!=nullptr){
            delete (table[i]);
        }
    }
    delete[] table;
}

template<class T>
void HashTable<T>::Insert(T element){
    int index= HashFunction(element);//what function give the key?
    if(table[index]==nullptr){
        table[index]=new Node(element);
    }
    else{
        Node* temp=table[index];
        table[index]=new Node(element,temp);
    }
    elements_num++;
    if(table_size==elements_num || table_size>=4*elements_num){
        this->Resize();
    }
}

template<class T>
Node<T>* HashTable<T>::Find(T element){
    int index= HashFunction(element);
    Node<T>* curr=table[index];
    if(table[index]==nullptr){
        return nullptr;
    }
    else{
        while (curr->next!=nullptr)
        {
            if(curr->element==element){
                return curr;
            }
            curr=curr->next;
        }
        return curr;//is nullptr
    }
}

template<class T>
void HashTable<T>::remove(T element){
    int index= HashFunction(element);
    Node<T>* curr=table[index];
    Node<t>* prev = table[index];
    if(table[index]->element==element){//if its the head
        Node<T>* temp=curr->next;
        delete curr;
        table[index]=temp;
    }
    else{
        curr=curr->next;
        while (curr->element!=element)
        {   prev=prev->next;
            curr=curr->next;
        }
        prev->next=curr->next;
        delete curr;
    }
    elements_num--;
    if(table_size==elements_num || table_size>=4*elements_num){
        this->Resize();
    }
}

template<class T>
void HashTable<T>::Resize(){
    if(table_size==elements_num){
       Node<T>** newTable = new Node<T>* [2*table_size]{nullptr};
       for(int i =0; i<table_size;i++){
           Node<T>* curr=table[i];
           if(table[i]!=nullptr){

           }
       }
    }
}

template<class T>
int HashTable<T>::HashFunction(int key){
    return (key % table_size);
}



#endif //WET2_HASHTABLE_H