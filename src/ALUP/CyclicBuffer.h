#ifndef CYCLIC_BUFFER_H
#define CYCLIC_BUFFER_H

#include <Arduino.h>

/**
 * A cyclic buffer with fixed size containing pointers to objects of type T
 */
template <class T, size_t SIZE> class CyclicBuffer
{
protected:    
    T* buffer[SIZE];
    // index of the next free space in the buffer
    size_t head = 0;
    // index of the oldest element in the buffer
    size_t tail = 0;
    // current number of elements in the buffer
    size_t items = 0;

public:
    bool IsFull()
    {
        return items == SIZE;
    }

    /**
     * Get the number of items currently in the buffer
     */
    size_t Items()
    {
        return this->items;
    }

    /**
     * Add the pointer to the given item to the buffer.
     * @param item_ptr: a pointer to the item to add
     * @return: 0 if appended successfully, 1 if failed (buffer is full)
     */
    int Append(T* item_ptr)
    {
        // check if we have space left
        if(this->IsFull())
        {
           return 1;
        }

        // add the new pointer to the buffer
        this->buffer[this->head] = item_ptr;
        this->head = (this->head + 1) % SIZE;;
        this->items += 1;

        return 0;
    }

    /**
     * get the oldest element in the buffer without removing it
     * @return: A pointer to the oldest element
     */
    T* Peek()
    {
        return this->buffer[this->tail];  
    }

    /**
     * return and remove the oldest element
     * in the buffer
     */
    T* Pop()
    {
        // check if there is an item in the buffer
        if(this->items == 0)
        {
            return nullptr;
        }
        // remove item and return it
        size_t old_tail = this->tail;
        this->tail = (this->tail + 1) % SIZE;
        this->items -= 1;
        return this->buffer[old_tail];
    }

};
#endif