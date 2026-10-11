// AVL-tree.
// добавление элемента,
// поиск элемента,
// удаление элемента с сохранением структуры дерева,
// чтение.

#pragma once

#include "queue.h"

#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <string>

template <typename T>
struct TNode {
    T data;
    TNode<T>* left;
    TNode<T>* right;
    int height;

    TNode(const T& data) {
        this -> data = data;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

template <typename T>
struct AVLTree {
    TNode<T>* root;

    AVLTree() {
        root = nullptr;
    }

    ~AVLTree() {
        clear();
    }

    void clear() {
        clearFrom(root);
        root = nullptr;
    }

    void clearFrom(TNode<T>* node) {
        if (node == nullptr) return;
        clearFrom(node -> left);
        clearFrom(node -> right);
        delete node;
    }

    int height(TNode<T>* node) {
        if (node == nullptr) return 0;
        return node -> height;
    }

    void updateHeight(TNode<T>* node) {
        int leftH = height(node -> left);
        int rightH = height(node -> right);
        node -> height = (leftH > rightH ? leftH : rightH) + 1;
    }

    int balanceFactor(TNode<T>* node) {
        return height(node -> left) - height(node -> right);
    }

    TNode<T>* rotateRight(TNode<T>* node) {
        TNode<T>* newRoot = node -> left;
        node -> left = newRoot -> right;
        newRoot -> right = node;

        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    TNode<T>* rotateLeft(TNode<T>* node) {
        TNode<T>* newRoot = node -> right;
        node -> right = newRoot -> left;
        newRoot -> left = node;

        updateHeight(node);
        updateHeight(newRoot);
        return newRoot;
    }

    TNode<T>* balance(TNode<T>* node) {
        updateHeight(node);

        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node -> left) < 0) {
                node -> left = rotateLeft(node -> left);
            }
            return rotateRight(node);
        }

        if (bf < -1) {
            if (balanceFactor(node -> right) > 0) {
                node -> right = rotateRight(node -> right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    void add(const T& data) {
        root = addFrom(root, data);
    }

    TNode<T>* addFrom(TNode<T>* node, const T& data) {
        if (node == nullptr) {
            return new TNode<T>(data);
        }

        if (data < node -> data) {
            node -> left = addFrom(node -> left, data);
        } else if (data > node -> data) {
            node -> right = addFrom(node -> right, data);
        } else {
            return node;
        }

        return balance(node);
    }

    bool find(const T& data) {
        TNode<T>* curNode = root;
        while (curNode != nullptr) {
            if (data == curNode -> data) {
                return true;
            } else if (data < curNode -> data) {
                curNode = curNode -> left;
            } else {
                curNode = curNode -> right;
            }
        }
        return false;
    }

    void remove(const T& data) {
        root = removeFrom(root, data);
    }

    TNode<T>* removeFrom(TNode<T>* node, const T& data) {
        if (node == nullptr) return nullptr;

        if (data < node -> data) {
            node -> left = removeFrom(node -> left, data);
        } else if (data > node -> data) {
            node -> right = removeFrom(node -> right, data);
        } else {
            if (node -> left == nullptr || node -> right == nullptr) {
                TNode<T>* child = node -> left != nullptr ? node -> left : node -> right;
                delete node;
                return child;
            }

            TNode<T>* minNode = findMin(node -> right);
            node -> data = minNode -> data;
            node -> right = removeFrom(node -> right, minNode -> data);
        }

        return balance(node);
    }

    TNode<T>* findMin(TNode<T>* node) {
        while (node -> left != nullptr) {
            node = node -> left;
        }
        return node;
    }

    Queue<T> bfs() const {
        Queue<T> allNodes;
        if (root == nullptr) return allNodes;

        Queue<TNode<T>*> level;
        level.push(root);

        while (!level.isEmpty()) {
            TNode<T>* curNode = level.front();
            level.pop();

            allNodes.push(curNode -> data);

            if (curNode -> left != nullptr) level.push(curNode -> left);
            if (curNode -> right != nullptr) level.push(curNode -> right);
        }

        return allNodes;
    }

    void print() {
        printFrom(root, "", false);
    }

    void printFrom(TNode<T>* node, std::string prefix, bool isLeft) {
        if (node == nullptr) return;

        std::cout << prefix;
        std::cout << (isLeft ? "|—" : "∟—");
        std::cout << node -> data << "\n";

        std::string newPrefix = prefix + (isLeft ? "│ " : "  ");

        bool hasLeft = node -> left != nullptr;
        bool hasRight = node -> right != nullptr;

        if (hasLeft && hasRight) {
            printFrom(node -> left, newPrefix, true);
            printFrom(node -> right, newPrefix, false);
        } else if (hasLeft) {
            printFrom(node -> left, newPrefix, false);
        } else if (hasRight) {
            printFrom(node -> right, newPrefix, false);
        }
    }

    void save(std::ostream& os) const {
        Queue<T> q = bfs();
        while (!q.isEmpty()) {
            os << q.front() << "\n";
            q.pop();
        }
    }
};