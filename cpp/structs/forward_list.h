// Односвязный список.
// добавление и удаление (по индексу и по краям),
// чтение (по индексу, по краям),
// удаление по значению,
// поиск по значению.

#pragma once

#include "node.h"

#include <cstddef>
#include <stdexcept>
#include <iostream>

template <typename T>
struct ForwardList {
    ForwardNode<T>* head = nullptr;
    ForwardNode<T>* tail = nullptr;

    ForwardList() {
        head = nullptr;
        tail = nullptr;
    }

    ~ForwardList() {
        while (head != nullptr) {
            ForwardNode<T>* next = head->next;
            delete head;
            head = next;
        }
        tail = nullptr;
    }


    void pushFront(const T& data) {
        ForwardNode<T>* newNode = new ForwardNode<T>(data, head);
        head = newNode;
        if (tail == nullptr) tail = newNode;
    }

    void pushBack(const T& data) {
        ForwardNode<T>* newNode = new ForwardNode<T>(data, nullptr);
        if (head != nullptr) {
            tail -> next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    void insertAfter(const T& data, const T& prevData) {
        ForwardNode<T>* node = find(prevData);
        if (node == nullptr) 
            throw std::invalid_argument("position element not found");

        ForwardNode<T>* newNode = new ForwardNode<T>(data, node -> next);
        node -> next = newNode;

        if (newNode -> next == nullptr) tail = newNode;
    }

    void insertBefore(const T& data, const T& prevData) {
        ForwardNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        }

        else if (node == head) {
            ForwardNode<T>* newNode = new ForwardNode<T>(data, node);
            head = newNode;
        } 
        
        else {
            ForwardNode<T>* curNode = head;
            while (curNode != nullptr && curNode -> next != node) {
                curNode = curNode -> next;
            }
            if (curNode == nullptr) {
                throw std::invalid_argument("node not in list");
            }

            ForwardNode<T>* newNode = new ForwardNode<T>(data, node);
            curNode->next = newNode;
        }   
    }


    void popFront() {
        if (head == nullptr) {
            throw std::out_of_range("list is empty");
        }

        ForwardNode<T>* delNode = head;
        head = delNode -> next;
        delete delNode;

        if (head == nullptr) tail = nullptr;
    }

    void popBack() {
        if (head == nullptr) {
            throw std::out_of_range("list is empty");
        }

        if (head == tail) {
            delete head;
            head = tail = nullptr;
        } else {
            ForwardNode<T>* prevNode = head;
            while (prevNode -> next != tail) {
                prevNode = prevNode -> next;
            }

            delete tail;
            tail = prevNode;
            tail -> next = nullptr;
        }
    }

    void removeAfter(const T& prevData) {
        ForwardNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        } 
        else if (node -> next == nullptr) {
            throw std::out_of_range("no node after this one");
        }

        if (node -> next == tail) {
            tail = node;
        }
        ForwardNode<T>* delNode = node -> next;
        node -> next = delNode -> next;
        delete delNode;
    }

    void removeBefore(const T& prevData) {
        ForwardNode<T>* node = find(prevData);
        if (node == nullptr) {
            throw std::invalid_argument("position element not found");
        } 
        else if (node == head) {
            throw std::out_of_range("no node before this one");
        }
        
        if (node == head -> next) {
            delete head;
            head = node;
        }
        else {
            ForwardNode<T>* curNode = head;
            while (curNode -> next != nullptr && curNode -> next -> next != node) {
                curNode = curNode -> next;
            }
            if (curNode -> next == nullptr) {
                throw std::invalid_argument("node not in list");
            }

            delete curNode -> next;
            curNode -> next = node;
        }   
    }


    bool deleteValue(const T& data) {
        if (head == nullptr) return false;

        if (head -> data == data) {
            popFront();
            return true;
        }

        ForwardNode<T>* curNode = head;
        while (curNode -> next != nullptr && curNode -> next -> data != data) {
            curNode = curNode -> next;
        }

        if (curNode -> next == nullptr) return false;

        ForwardNode<T>* delNode = curNode -> next;
        if (delNode == tail) tail = curNode;
        curNode -> next = delNode -> next;
        delete delNode;
        return true;
    }


    T& operator[](std::size_t index) {
        if (index >= getSize()) {
            throw std::out_of_range("index out of range");
        }

        ForwardNode<T>* curNode = head;
        for (std::size_t i = 0; i < index; i++) {
            curNode = curNode -> next;
        }
        return curNode -> data;
    }

    T& front() {
        if (head == nullptr) throw std::out_of_range("empty list");
        return head -> data;
    }

    T& back() {
        if (tail == nullptr) throw std::out_of_range("empty list");
        return tail -> data;
    }


    ForwardNode<T>* find(const T& data) {
        ForwardNode<T>* curNode = head;
        while (curNode != nullptr && curNode -> data != data) {
            curNode = curNode -> next;
        }
        return curNode;
    }


    std::size_t getSize() const {
        std::size_t cnt = 0;
        ForwardNode<T>* cur = head;
        while (cur != nullptr) {
            cnt++;
            cur = cur -> next;
        }
        return cnt;
    }

    void print() {
        ForwardNode<T>* cur = head;
        while (cur != nullptr) {
            std::cout << cur -> data << " ";
            cur = cur -> next;
        }
        std::cout << "\n";
    }

    void save(std::ostream& os) const {
        ForwardNode<T>* cur = head;
        while (cur != nullptr) {
            os << cur -> data << "\n";
            cur = cur -> next;
        }
    }
};