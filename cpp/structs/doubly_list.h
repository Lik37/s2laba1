// Двусвязный список.
// добавление и удаление (4 способа: до, после, голова, хвост),
// чтение (по индексу, по краям),
// удаление по значению,
// поиск по значению.

#pragma once

#include "node.h"

#include <stdexcept>
#include <iostream>

template <typename T>
struct DoublyList {
    DoublyNode<T>* head = nullptr;
    DoublyNode<T>* tail = nullptr;

    DoublyList() {
        head = nullptr;
        tail = nullptr;
    }

    ~DoublyList() {
        while (head != nullptr) {
            DoublyNode<T>* next = head -> next;
            delete head;
            head = next;
        }
        tail = nullptr;
    }


    void pushFront(const T& data) {
        DoublyNode<T>* newNode = new DoublyNode<T>(data, head, nullptr);

        if (head != nullptr) {
            head -> prev = newNode;
        } else {
            tail = newNode;
        }
        head = newNode;
    }

    void pushBack(const T& data) {
        DoublyNode<T>* newNode = new DoublyNode<T>(data, nullptr, tail);

        if (tail != nullptr) {
            tail -> next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    void insertAfter(const T& data, const T& prevData) {
        DoublyNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        }

        DoublyNode<T>* newNode = new DoublyNode<T>(data, node -> next, node);

        if (node -> next != nullptr) {
            node -> next -> prev = newNode;
        } else {
            tail = newNode;
        }
        node -> next = newNode;
    }

    void insertBefore(const T& data, const T& prevData) {
        DoublyNode<T>* node = find(prevData);
        if (node == nullptr) 
            throw std::invalid_argument("position element not found");


        DoublyNode<T>* newNode = new DoublyNode<T>(data, node, node -> prev);

        if (node -> prev != nullptr) {
            node -> prev -> next = newNode;
        } else {
            head = newNode;
        }
        node -> prev = newNode;
    }


    void popFront() {
        if (head == nullptr) {
            throw std::out_of_range("list is empty");
        }

        DoublyNode<T>* delNode = head;
        head = delNode -> next;

        if (head != nullptr) {
            head -> prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete delNode;
    }

    void popBack() {
        if (tail == nullptr) {
            throw std::out_of_range("list is empty");
        }

        DoublyNode<T>* delNode = tail;
        tail = delNode -> prev;

        if (tail != nullptr) {
            tail -> next = nullptr;
        } else {
            head = nullptr;
        }
        delete delNode;
    }

    void removeAfter(const T& prevData) {
        DoublyNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        }
        if (node -> next == nullptr) {
            throw std::out_of_range("no node after this one");
        }

        DoublyNode<T>* delNode = node -> next;
        node -> next = delNode -> next;

        if (delNode -> next != nullptr) {
            delNode -> next -> prev = node;
        } else {
            tail = node;
        }
        delete delNode;
    }


    void removeBefore(const T& prevData) {
        DoublyNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        }
        if (node -> prev == nullptr) {
            throw std::out_of_range("no node before this one");
        }

        DoublyNode<T>* delNode = node -> prev;
        node -> prev = delNode -> prev;

        if (delNode -> prev != nullptr) {
            delNode -> prev -> next = node;
        } else {
            head = node;
        }
        delete delNode;
    }


    bool deleteValue(const T& data) {
        DoublyNode<T>* curNode = head;
        while (curNode != nullptr && curNode -> data != data) {
            curNode = curNode -> next;
        }
        if (curNode == nullptr) {
            return false;
        }

        if (curNode -> prev != nullptr) {
            curNode -> prev -> next = curNode -> next;
        } else {
            head = curNode -> next;
        }

        if (curNode -> next != nullptr) {
            curNode -> next -> prev = curNode -> prev;
        } else {
            tail = curNode -> prev;
        }

        delete curNode;
        return true;
    }


    T& operator[](std::size_t index) {
        if (index >= getSize()) {
            throw std::out_of_range("index out of range");
        }

        DoublyNode<T>* curNode = head;
        for (std::size_t i = 0; i < index; i++) {
            curNode = curNode -> next;
        }
        return curNode -> data;
    }

    T& front() {
        if (head == nullptr)
            throw std::out_of_range("empty list");
        return head -> data;
    }

    T& back() {
        if (tail == nullptr)
            throw std::out_of_range("empty list");
        return tail -> data;
    }


    DoublyNode<T>* find(const T& data) {
        DoublyNode<T>* curNode = head;
        while (curNode != nullptr && curNode -> data != data) {
            curNode = curNode -> next;
        }
        return curNode;
    }


    std::size_t getSize() const {
        std::size_t cnt = 0;
        DoublyNode<T>* cur = head;
        while (cur != nullptr) {
            cnt++;
            cur = cur -> next;
        }
        return cnt;
    }

    void print() {
        DoublyNode<T>* cur = head;
        while (cur != nullptr) {
            std::cout << cur -> data << " ";
            cur = cur -> next;
        }
        std::cout << "\n";
    }

    void save(std::ostream& os) const {
        DoublyNode<T>* cur = head;
        while (cur != nullptr) {
            os << cur -> data << "\n";
            cur = cur -> next;
        }
    }
};