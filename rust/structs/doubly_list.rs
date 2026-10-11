// Двусвязный список.
use crate::structs::node::DoublyNode;
use std::io::Write;
use std::ptr;

pub struct DoublyList<T> {
    pub head: *mut DoublyNode<T>,
    pub tail: *mut DoublyNode<T>,
}

impl<T: PartialEq> DoublyList<T> {
    pub fn new() -> Self {
        Self { head: ptr::null_mut(), tail: ptr::null_mut() }
    }

    pub fn pushFront(&mut self, data: T) {
        let new_ptr = Box::into_raw(Box::new(DoublyNode::new(data, self.head, ptr::null_mut())));
        if !self.head.is_null() {
            unsafe { (*self.head).prev = new_ptr; }
        } else {
            self.tail = new_ptr;
        }
        self.head = new_ptr;
    }

    pub fn pushBack(&mut self, data: T) {
        let new_ptr = Box::into_raw(Box::new(DoublyNode::new(data, ptr::null_mut(), self.tail)));
        if !self.tail.is_null() {
            unsafe { (*self.tail).next = new_ptr; }
        } else {
            self.head = new_ptr;
        }
        self.tail = new_ptr;
    }

    pub fn insertAfter(&mut self, data: T, prev_data: &T) {
        let node = self.find(prev_data);
        if node.is_null() {
            panic!("position element not found");
        }
        unsafe {
            let new_ptr = Box::into_raw(Box::new(DoublyNode::new(data, (*node).next, node)));
            if !(*node).next.is_null() {
                (*(*node).next).prev = new_ptr;
            } else {
                self.tail = new_ptr;
            }
            (*node).next = new_ptr;
        }
    }

    pub fn insertBefore(&mut self, data: T, pos_data: &T) {
        let node = self.find(pos_data);
        if node.is_null() {
            panic!("position element not found");
        }
        unsafe {
            let new_ptr = Box::into_raw(Box::new(DoublyNode::new(data, node, (*node).prev)));
            if !(*node).prev.is_null() {
                (*(*node).prev).next = new_ptr;
            } else {
                self.head = new_ptr;
            }
            (*node).prev = new_ptr;
        }
    }

    pub fn popFront(&mut self) {
        if self.head.is_null() {
            panic!("list is empty");
        }
        unsafe {
            let del = Box::from_raw(self.head);
            self.head = (*del).next;
            if !self.head.is_null() {
                (*self.head).prev = ptr::null_mut();
            } else {
                self.tail = ptr::null_mut();
            }
        }
    }

    pub fn popBack(&mut self) {
        if self.tail.is_null() {
            panic!("list is empty");
        }
        unsafe {
            let del = Box::from_raw(self.tail);
            self.tail = del.prev;
            if !self.tail.is_null() {
                (*self.tail).next = ptr::null_mut();
            } else {
                self.head = ptr::null_mut();
            }
        }
    }

    pub fn removeAfter(&mut self, prev_data: &T) {
        let node = self.find(prev_data);
        if node.is_null() {
            panic!("position element not found");
        }
        unsafe {
            if (*node).next.is_null() {
                panic!("no node after this one");
            }
            let del = Box::from_raw((*node).next);
            (*node).next = (*del).next;
            if !del.next.is_null() {
                (*del.next).prev = node;
            } else {
                self.tail = node;
            }
        }
    }

    pub fn removeBefore(&mut self, pos_data: &T) {
        let node = self.find(pos_data);
        if node.is_null() {
            panic!("position element not found");
        }
        unsafe {
            if (*node).prev.is_null() {
                panic!("no node before this one");
            }
            let del = Box::from_raw((*node).prev);
            (*node).prev = del.prev;
            if !del.prev.is_null() {
                (*del.prev).next = node;
            } else {
                self.head = node;
            }
        }
    }

    pub fn deleteValue(&mut self, data: &T) -> bool {
        let node = self.find(data);
        if node.is_null() { return false; }
        unsafe {
            if !(*node).prev.is_null() {
                (*(*node).prev).next = (*node).next;
            } else {
                self.head = (*node).next;
            }
            if !(*node).next.is_null() {
                (*(*node).next).prev = (*node).prev;
            } else {
                self.tail = (*node).prev;
            }
            drop(Box::from_raw(node));
            true
        }
    }

    pub fn get(&self, index: usize) -> &T {
        if index >= self.getSize() {
            panic!("index out of range");
        }
        let mut cur = self.head;
        for _ in 0..index {
            unsafe { cur = (*cur).next; }
        }
        unsafe { &(*cur).data }
    }

    pub fn front(&self) -> &T {
        if self.head.is_null() {
            panic!("empty list");
        }
        unsafe { &(*self.head).data }
    }

    pub fn back(&self) -> &T {
        if self.tail.is_null() {
            panic!("empty list");
        }
        unsafe { &(*self.tail).data }
    }

    pub fn find(&self, data: &T) -> *mut DoublyNode<T> {
        let mut cur = self.head;
        while !cur.is_null() {
            unsafe {
                if (*cur).data == *data {
                    return cur;
                }
                cur = (*cur).next;
            }
        }
        ptr::null_mut()
    }

    pub fn getSize(&self) -> usize {
        let mut cnt = 0;
        let mut cur = self.head;
        while !cur.is_null() {
            cnt += 1;
            unsafe { cur = (*cur).next; }
        }
        cnt
    }

    pub fn print(&self) where T: std::fmt::Display {
        let mut cur = self.head;
        while !cur.is_null() {
            unsafe {
                print!("{} ", (*cur).data);
                cur = (*cur).next;
            }
        }
        println!();
    }

    pub fn save(&self, os: &mut impl Write) where T: std::fmt::Display {
        let mut cur = self.head;
        while !cur.is_null() {
            unsafe {
                writeln!(os, "{}", (*cur).data).unwrap();
                cur = (*cur).next;
            }
        }
    }
}

impl<T> Drop for DoublyList<T> {
    fn drop(&mut self) {
        while !self.head.is_null() {
            unsafe {
                let del = Box::from_raw(self.head);
                self.head = (*del).next;
            }
        }
    }
}