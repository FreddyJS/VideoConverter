// Returns the pointer to the LinkedListItem
// Returns a null pointer if the item on the position requested not exists
// \@index: number of the position of the desired item
//
// Implements a current pointer to improve the time of the searchs
template <class T>
LinkedListItem<T>* LinkedList<T>::getItemPtr(size_t index) {
    if (this->first == NULL) return NULL;

    LinkedListItem<T>* p = this->current;
    LinkedListItem<T>* paux = NULL;
    size_t n = this->index;

    if (p == NULL) {
        p = this->first;
        n = 0;
    }

    if (n == index) {
        this->current = p;
        this->index = n;
        return p;
    }

    if (n < index) {
        while ( (paux = p->next) ) {
            n++;
            p = paux;
            if (n == index) {
                this->current = p;
                this->index = n;
                return p;
            }
        }

    } else {
        while ( (paux = p->previous) ) {
            n--;
            p = paux;
            if (n == index) {
                this->current = p;
                this->index = n;
                return p;
            }
        }
    }

    return NULL;
}


// Adds an item to the back of the list
// \@data: the data to store
template <class T>
void LinkedList<T>::addLast(T data) {
    LinkedListItem<T>* item;
    if ( this->first == NULL && this->last == NULL ) {
        item = new LinkedListItem<T>(data);
        item->setData(data);
        this->first = item;
        this->last = item;

        this->size++;

    } else {
        item = new LinkedListItem<T>(data);
        (this->last)->next = item;
        item->previous = this->last;

        this->last = item;
        this->size++;

    }
}

// Returns the first item of the list
//
// Throws exception if the list is empty
template <class T>
T LinkedList<T>::getFirst() {
    if (this->first != NULL) {
        T data = this->first->getData();
        return data;
    } else {
        std::string msg = "LinkedListException: Item not found! Operation: getLast()";
        throw LinkedListException(msg);
    }
}

// Returns the last item of the list
//
// Throws exception if the list is empty
template <class T>
T LinkedList<T>::getLast() {
    if (this->last != NULL) {
        T data = this->last->getData();
        return data;
    } else {
        std::string msg = "LinkedListException: Item not found! Operation: getLast()";
        throw LinkedListException(msg);
    }
}


// Returns the requested item on the position [index]
// Range [0, list.size-1] if the list is not empty
// \@index: number of the position of the desired item
//
// Throws exception if the item not exists
template <class T>
T LinkedList<T>::get(size_t index){
    LinkedListItem<T>* p = this->getItemPtr(index);

    if ( p == NULL ) {
        std::string msg = "LinkedListException: Item not found! Operation: get()";
        throw LinkedListException(msg);
    } else {
        return p->getData();
    }    

}

// Replaces the data of the item on the position [index]
// Range [0, list.size-1] if the list is not empty
//
// \@index: number of the position of the item to change
// \@data: the data to store
//
// Throws exception if the item not exists
template <class T>
void LinkedList<T>::set(size_t index, T data){
    LinkedListItem<T>* p = this->getItemPtr(index);

    if ( p == NULL ) {
        std::string msg = "LinkedListException: Item not found! Operation: set(" + std::to_string(index) +")";
        throw LinkedListException(msg);
    } else {
        p->setData(data);
    }    

}


// Remove the item on the position [index] if exists
// \@index: number of the position of the item to remove
//
// Returns true if success
template <class T>
bool LinkedList<T>::remove(size_t index) {
    LinkedListItem<T>* p = this->getItemPtr(index);

    if (p == NULL) return false;

    this->current = p->previous;
    if (this->index != 0) {
        this->index--;
    }

    if (p == this->first && p == this->last) {
        this->current = NULL;
        this->index = 0;

        this->size--;
        this->first = NULL;
        this->last = NULL;

        delete p;
        return true;
    }

    if (p == this->last) {
        this->last = p->previous;
        this->size--;

        (p->previous)->next = NULL;

        delete p;
        return true;
    } else if ( p == this->first) {
        this->first = p->next;
        this->size--;

        (p->next)->previous = NULL;

        delete p;
        return true;
    } else {
        this->size--;

        (p->next)->previous = p->previous;
        (p->previous)->next = p->next;

        delete p;
        return true;
    }

    return false;
}


// Clears the list by calling the destructor of any Item stored
//
// Automatic called when the list is destroyed
template <class T>
void LinkedList<T>::clear() {
    LinkedListItem<T>* p;
    LinkedListItem<T>* paux;

    if (this->size > 0) {
        p = this->last;

        while ( (paux = p->previous) ) {
            delete p;
            this->size--;
            p = paux;

            this->last = p;
        }

        p = this->first;
        delete p;
        this->size--;
        
        this->last = NULL;
        this->first = NULL;
        
        this->current = NULL;
        this->index = 0;
    }
}



// Shifts the items of the list to the right one position
// Cyclic Shift Right
// Returns true if success
template <class T>
bool LinkedList<T>::shiftr() {
    if (this->size >= 2) {
        LinkedListItem<T>* pfirst = this->first;
        LinkedListItem<T>* plast = this->last;

        this->first = plast;
        this->last = plast->previous;

        (plast->previous)->next = NULL;
        plast->previous = NULL;
        plast->next = pfirst;
        pfirst->previous = plast;

        if (this->current != NULL) {
            if (this->index == this->size-1) {
                this->index = 0;
            } else {
                this->index++;
            }
        }
        
        return true;
    } else {
        return false;
    }
    return false;
}



// Shifts the items of the list to the right N times
// Cyclic Shift Right
// \@shifts: the number of shifts to do
//
// Returns true if success
template <class T>
bool LinkedList<T>::shiftr(size_t shifts) {
    for (size_t i = 0; i < shifts; i++) {
        bool success = this->shiftr();
        if (!success) return false;
    }

    return true;
}

// Shifts the items of the list to the left one position
// Cyclic Shift Left
// Returns true if success
template <class T>
bool LinkedList<T>::shiftl() {
    if (this->size >= 2) {
        LinkedListItem<T>* pfirst = this->first;
        LinkedListItem<T>* plast = this->last;

        this->first = pfirst->next;
        this->last = pfirst;

        (pfirst->next)->previous = NULL;
        plast->next = pfirst;
        pfirst->previous = plast;
        pfirst->next = NULL;


        if (this->current != NULL) {
            if (this->index == 0) {
                this->index = this->size-1;
            } else {
                this->index--;
            }
        }

        return true;
    } else {
        return false;
    }
    return false;
}



// Shifts the items of the list to the left one position
// Cyclic Shift Left
// \@shifts: the number of shifts to do
//
// Returns true if success
template <class T>
bool LinkedList<T>::shiftl(size_t shifts) {
    for (size_t i = 0; i < shifts; i++) {
        bool success = this->shiftl();
        if (!success) return false;
    }

    return true;
}

// Copy all the items from one list to the new list
// Used on the copy constructor
// \@list: LinkedList pointer to the list to copy
//
// Allocates new memory for every item on the new list
template <class T>
void LinkedList<T>::copyLinkedList(LinkedList<T>* list) {
    for (size_t i = 0; i < list->size; i++) {
        this->addLast(list->get(i));
    }    
}

// Overloading operator new
template <class T>
void* LinkedList<T>::operator new(size_t size) {
    return malloc(size);
}

// Overloading operator delete
template <class T>
void LinkedList<T>::operator delete(void* ptr) {
    LinkedList<T>* list = (LinkedList<T>*) ptr;
    list->clear();
    free(ptr);
}
