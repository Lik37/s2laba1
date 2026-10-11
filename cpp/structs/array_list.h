// Массив. 
// создание, 
// добавление элемента (по индексу и в конец массива)
// получение элемента по индексу, 
// удаление элемента по индексу, 
// замена элемента по индексу, 
// длина массива, 
// чтение.
#pragma once

#include <cstddef>
#include <stdexcept>
#include <iostream>

template <typename T>
struct ArrayList 
{
    T* data;
    std::size_t size;
    std::size_t capacity;

    ArrayList() {
        data = new T[0];
        size = 0;
        capacity = 0;
    }

    ~ArrayList() {
        delete[] data;
    }

    void pushBack(const T& value) {
        if (size < capacity) {
            data[size] = value;
            size++;
        } else {
            capacity++;
            T* newData = new T[capacity];
            for (std::size_t i = 0; i < size; i++)
                newData[i] = data[i];
            
            delete[] data;
            data = newData;

            data[size] = value;
            size++;
        }
    }

    void insert(const T& value, std::size_t index) {
        if (index > size) {
            throw std::out_of_range("index out of range");
        } else if (index == size) {
            pushBack(value);
        } else
        {
            if (size < capacity) {
                for (std::size_t i = size; i > index; i--) {
                    data[i] = data[i - 1];
                }
                data[index] = value;
                size++;

            } else {
                capacity++;
                T* newData = new T[capacity];
                for (std::size_t i = 0; i < index; i++)
                    newData[i] = data[i];
                newData[index] = value;
                for (std::size_t i = index; i < size; i++) 
                    newData[i + 1] = data[i];
                
                delete[] data;
                data = newData;
                size++;
            }
        }
    }

    T& operator[](std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("index out of range");
        }
        return data[index];
    }

    void remove(std::size_t index) {
        if (index >= size) {
            throw std::out_of_range("index out of range");
        }
        for (std::size_t i = index + 1; i < size; i++) {
            data[i - 1] = data[i];
        }
        size--;
    }

    std::size_t getSize() const {
        return size;
    }

    void print() {
        for (std::size_t i = 0; i < size; i++) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }

    void save(std::ostream& os) const {
        for (std::size_t i = 0; i < size; i++) {
            os << data[i] << "\n";
        }
    }
};
