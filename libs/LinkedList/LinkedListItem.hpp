#ifndef _LinkedListItem_H
#define _LinkedListItem_H

template <class T>
class LinkedListItem {
    T data;

    public:
        LinkedListItem<T>* next;
        LinkedListItem<T>* previous;

        LinkedListItem() {
            this->next = 0;
            this->previous = 0;
        }

        LinkedListItem(T data) {
            this->next = 0;
            this->previous = 0;
            this->data = data;
        }

        ~LinkedListItem() { }

        T getData();
        T* getDataPtr();
        void setData(T data);

};

// Returns the data stored on the Item
template <class T>
T LinkedListItem<T>::getData() {
    return this->data;
}

// Returns de pointer to the data stored on the Item
template <class T>
T* LinkedListItem<T>::getDataPtr() {
    return &(this->data);
}

// Sets the data stored on the Item
template <class T>
void LinkedListItem<T>::setData(T data) {
    this->data = data;
}

#endif