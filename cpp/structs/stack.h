// Стек.
// добавление и удаление элемента (push и pop),
// чтение.

#pragma once

#include "node.h"

#include <stdexcept>
#include <iostream>

template <typename T>
struct Stack {
    ForwardNode<T>* head = nullptr;

    Stack() {
        head = nullptr;
    }

    ~Stack() {
        while (head != nullptr) {
            ForwardNode<T>* next = head -> next;
            delete head;
            head = next;
        }
    }

    void push(const T& data) {
        ForwardNode<T>* newNode = new ForwardNode<T>(data, head);
        head = newNode;
    }

    T pop() {
        if (head == nullptr) {
            throw std::out_of_range("stack is empty");
        }

        ForwardNode<T>* delNode = head;
        head = delNode -> next;
        T data = delNode -> data;
        delete delNode;
        return data;
    }

    T& front() {
        if (head == nullptr) {
            throw std::out_of_range("stack is empty");
        }
        return head -> data;
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

    void save(std::ostream& os) const{
        ForwardNode<T>* cur = head;
        while (cur != nullptr) {
            os << cur -> data << "\n";
            cur = cur -> next;
        }
    }
};