// Очередь.
// добавление и удаление элемента (push и pop),
// чтение.

#pragma once

#include "node.h"

#include <stdexcept>
#include <iostream>

template <typename T>
struct Queue {
    ForwardNode<T>* head = nullptr;
    ForwardNode<T>* tail = nullptr;

    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    ~Queue() {
        while (head != nullptr) {
            ForwardNode<T>* next = head -> next;
            delete head;
            head = next;
        }
        tail = nullptr;
    }

    void push(const T& data) {
        ForwardNode<T>* newNode = new ForwardNode<T>(data, nullptr);

        if (tail != nullptr) {
            tail -> next = newNode;
        } else {
            head = newNode;
        }
        tail = newNode;
    }

    T pop() {
        if (head == nullptr) {
            throw std::out_of_range("queue is empty");
        }

        ForwardNode<T>* delNode = head;
        head = delNode -> next;
        T data = delNode -> data;
        delete delNode;

        if (head == nullptr) {
            tail = nullptr;
        }
        return data;
    }

    T& front() {
        if (head == nullptr) {
            throw std::out_of_range("queue is empty");
        }
        return head -> data;
    }

    T& back() {
        if (tail == nullptr) {
            throw std::out_of_range("queue is empty");
        }
        return tail -> data;
    }

    bool isEmpty() {
        return head == nullptr;
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