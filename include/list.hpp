#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include "hashentry.hpp"
#include "memoryentry.hpp"
#include "list.hpp"

template <typename T>
class listNode {
private:
    T* content;
    listNode<T> *next;
public:
    listNode(T *content);
    ~listNode();

    T *getContent();
    listNode<T> *getNext();

    void setNext(listNode<T> *newNext);
};

template <typename T>
class list {
private:
    listNode<T> *start;
    listNode<T> *end;
    int count;
public:
    list();
    ~list();

    int getCount();
    listNode<T> *getStart();
    listNode<T> *getEnd();
    void insert(T *content);

    void printAll();
    bool search(int test);
    void remove(int target);
    
    T *getContentByKey(int page);
};

//~~~~~~~~~~~~~~~~~~~listNode~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
listNode<T>::listNode(T* content) :
    content(content),
    next(NULL) {
}

template <typename T>
listNode<T>::~listNode() {}

template <typename T>
T *listNode<T>::getContent() { return content; }

template <typename T>
listNode<T>* listNode<T>::getNext() { return next; }

template <typename T>
void listNode<T>::setNext(listNode<T>* newNext) { next = newNext; }

//~~~~~~~~~~~~~~~~~~~~~~~list~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T>
list<T>::list() :
    start(NULL),
    end(NULL),
    count(0) {
}

template <typename T>
list<T>::~list() {
    listNode<T> *curr = start;
    while (curr != NULL) {
        listNode<T> *newcurr = curr->getNext();
        delete curr;
        curr = newcurr;
    }
}

template <typename T>
int list<T>::getCount() { return count; }

template <typename T>
listNode<T>* list<T>::getStart() { return start; }

template <typename T>
listNode<T>* list<T>::getEnd() { return end; }

template <typename T>
void list<T>::insert(T *content) {
//insert to the end of list
    listNode<T> *temp = new listNode<T>(content);
    if (start != NULL)
        end->setNext(temp);
    else
        start = temp;
    end = temp;
    count++;
}

template <typename T>
void list<T>::printAll() {
//print all elements in list, if pageNum can be printed
    listNode<T> *curr = start;
    std::cout << "List: ";
    for (int i = 0; i < count; ++i) {
        std::cout << curr->getPageNum() << " ";
        curr = curr->getNext();
    }
    std::cout << std::endl;
}

template <typename T>
bool list<T>::search(int test) {
//is something equal to test inside?
    listNode<T> *curr = start;
    for (int i = 0; i < count; ++i) {
        if (curr->getContent()->getPageNum() == test)
            return true;
        curr = curr->getNext();
    }
    return false;
}

template <typename T>
void list<T>::remove(int test) {
    listNode<T> *previous = NULL;
    listNode<T> *current = start;
    while (current != NULL) {
        if (current->getContent()->getPageNum() == test) {
            if (previous != NULL)
                previous->setNext(current->getNext());
            else
                start = current->getNext();

            if (end == current)
                end = previous;
            
            current->setNext(NULL);
            delete current;
            break;
        }
        previous = current;
        current = current->getNext();
    }
    count--;
    return;
}

template <typename T>
T* list<T>::getContentByKey(int page) {
/*get pointer to item with keyValue of testkey
  item must have function getKey*/
    listNode<T> *current = start;
    while (current != NULL) {
        if (page == current->getContent()->getPageNum())
            return current->getContent();
        else
            current = current->getNext();
    }
    return NULL;
}

#endif /* LIST_HPP */
