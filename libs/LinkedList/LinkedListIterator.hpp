#ifndef _LinkedListIterator_H
#define _LinkedListIterator_H


// Iterator class for the LinkedList
template <class T>
class LinkedListIterator{
    public:
        LinkedListItem<T>* p;
        LinkedListIterator(LinkedListItem<T>* p) : p(p) {}
        ~LinkedListIterator() { }

        bool operator!=(LinkedListIterator<T> rhs) {return p != rhs.p;}
        T operator*() {return p->getData();}
        void operator++() {p = p->next;}
};

#endif