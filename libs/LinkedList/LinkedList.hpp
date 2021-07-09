#ifndef _LinkedList_H
#define _LinkedList_H

#include <stddef.h>
#include <iostream>
#include <exception>

// Include the Item class, those will be the nodes that stores the data
#include "LinkedListItem.hpp"
// Include the Iterator class for the LinkedList, it allows to use for_range loops with the list
#include "LinkedListIterator.hpp"
// Include the Exception class for the LinkedList, it allows to use custom exception messages
#include "LinkedListException.hpp"

// LinkedList class, doubly linked list implementation
// If you use static allocation please be sure to return the list if its modified
// \@type: the type of the data to store
//
// LinkedList<type> list;
// LinkedList<type>* list = new LinkedList<int>();
template <class T>
class LinkedList{
    typedef T* iterator;

    private:
        void copyLinkedList(LinkedList<T>* list);
        
        LinkedListItem<T>* first = NULL;
        LinkedListItem<T>* last = NULL;

        size_t index = 0;

    public:
        LinkedListItem<T>* current = NULL;
        LinkedListItem<T>* getItemPtr(size_t index);
        // Empty constructor
        LinkedList() {  }

        // Copy constructor from another list of the same type of data
        LinkedList(LinkedList<T>* list) {
            this->copyLinkedList(list);

            this->current = NULL;
            this->index = 0;
        }
        
        ~LinkedList() { 
            clear();
        }

        size_t size = 0;

        void addLast(T data);
        T getFirst();
        T getLast();
        T get(size_t index);

        void set(size_t index, T data);

        bool remove(size_t index);
        void clear();

        bool shiftr();
        bool shiftr(size_t shifts);

        bool shiftl();
        bool shiftl(size_t shifts);

        LinkedListIterator<T> begin() { return LinkedListIterator<T>(first); }
        LinkedListIterator<T> end() { return NULL; }

        void* operator new(size_t size);
        void operator delete(void* list);
};

// Include all the definitions of the functions for the LinkedList class
#include "LinkedListImpl.hpp"

#endif
