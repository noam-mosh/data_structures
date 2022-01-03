
#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include "Node.h"
#define P 17

typedef enum {
    SUCCESS,
    FAILURE,
    ALLOCATION_ERROR,
    ALREADY_EXISTS,
    DO_NOT_EXIST
} HashStatus;

template<class T>
class HashTable{
    public:
    HashTable();
    ~HashTable();
    HashTable(const HashTable& other)=default;//check
    HashTable& operator=(const HashTable& other)=default;//check
    HashStatus Insert(T element);
    Node<T>* Find(T element);//returns the Node with the wanted data nullptr if it doesnt exist
    HashStatus remove(T element);//assuming the data exist
    //int getSize();

    //private:
    int elements_num;
    int table_size;
    Node<T>** table;
    void Resize();
    int HashFunction(int Key);
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

template<class T>//help function to insert into a list in an array
HashStatus InsertToTable(T element,int index, Node<T>** table){
    if(table[index]==nullptr){
        Node<T>* newNode=new Node<T>(element);
        if (!newNode){
            return ALLOCATION_ERROR;
        }
        table[index]=newNode;
    }
    else{
        Node<T>* temp=table[index];
        Node<T>* newNode=new Node<T>(element,temp);
        if (!newNode){
            return ALLOCATION_ERROR;
        }
        table[index]=newNode;
    }
    return SUCCESS;
}

template<class T>
HashStatus HashTable<T>::Insert(T element){
    if (this->Find(element)){
        return ALREADY_EXISTS;
    }
    int index= HashFunction(element.key());
    if (InsertToTable(element,index,table)==ALLOCATION_ERROR){
        return ALLOCATION_ERROR;
    }
    elements_num++;
    if(table_size==elements_num || table_size>=4*elements_num){
        this->Resize();
    }
    return SUCCESS;
}

template<class T>
Node<T>* HashTable<T>::Find(T element){
    int index= HashFunction(element.key());
   
    if(table[index]==nullptr){
        return nullptr;
    }
    else{
         Node<T>* curr=table[index];
        while (curr!=nullptr)
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
HashStatus HashTable<T>::remove(T element){
    if ((this->Find(element))==nullptr){
        return DO_NOT_EXIST;
    }
    
    int index= HashFunction(element.key());
    Node<T>* curr=table[index];
    Node<T>* prev = table[index];
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
    return SUCCESS;
}

template<class T>
void HashTable<T>::Resize(){
    if(table_size==elements_num){//needs to increase the array
       Node<T>** newTable = new Node<T>* [2*table_size]{nullptr};
       int old_table_size=table_size;
       table_size=table_size*2;
       for(int i =0; i<old_table_size;i++){
           Node<T>* curr=table[i];
           while (curr!=nullptr)
               {
                   InsertToTable(curr->element,HashFunction((curr->element).key()),newTable);
                   curr=curr->next;
               }
           }
        for(int i =0; i<old_table_size;i++){
           Node<T>* curr=table[i];
           while (curr!=nullptr){
               Node<T>* temp=curr->next;
               delete curr;
               curr=temp;
           }
           table[i]=nullptr;
       }
       delete[] table;
       table=newTable;
    }

    if(table_size>=4*elements_num && table_size>P){//needs to decrease the array
    int newSize = 0.5*table_size;
    Node<T>** newTable = new Node<T>* [newSize]{nullptr};
       int old_table_size=table_size;
       table_size=table_size*0.5;
       for(int i =0; i<old_table_size;i++){
           Node<T>* curr=table[i];
           while (curr!=nullptr)
               {
                   InsertToTable(curr->element,HashFunction(curr->element.key()),newTable);
               }
           }
        for(int i =0; i<old_table_size;i++){
           Node<T>* curr=table[i];
           while (curr!=nullptr){
               Node<T>* temp=curr->next;
               delete curr;
               curr=temp;
           }
           table[i]=nullptr;
       }
       delete[] table;
       table=newTable;
    }
}

template<class T>
int HashTable<T>::HashFunction(int Key){
    if(Key<0){
        Key=-Key;
    }
    return (Key % table_size);
}



#endif //WET2_HASHTABLE_H