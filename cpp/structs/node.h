#pragma once

template <typename T>
struct ForwardNode {
    T data;
    ForwardNode* next;

    ForwardNode() {
        next = nullptr;
    }

    ForwardNode(const T& data) {
        this -> data = data;
        next = nullptr;
    }

    ForwardNode(const T& data, ForwardNode* next) {
        this -> data = data;
        this -> next = next;
    }

};

template <typename T>
struct DoublyNode {
    T data;
    DoublyNode* next;
    DoublyNode* prev;


    DoublyNode() {
        next = nullptr;
        prev = nullptr;
    }

    DoublyNode(const T& data) {
        this -> data = data;
        next = nullptr;
        prev = nullptr;
    }

    DoublyNode(const T& data, DoublyNode* next, DoublyNode* prev) {
        this -> data = data;
        this -> next = next;
        this -> prev = prev;
    }
};
