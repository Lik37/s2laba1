use crate::structs::node::ForwardNode;
use std::io::Write;
use std::ptr;

pub struct ForwardList<T> {
    pub head: *mut ForwardNode<T>,
    pub tail: *mut ForwardNode<T>,
}

impl<T: PartialEq> ForwardList<T> {
    pub fn new() -> Self {
        Self { head: ptr::null_mut(), tail: ptr::null_mut() }
    }

    pub fn pushFront(&mut self, data: T) {
        let new_ptr = Box::into_raw(Box::new(ForwardNode::new(data, self.head)));
        self.head = new_ptr;
        if self.tail.is_null() {
            self.tail = new_ptr;
        }
    }

    pub fn pushBack(&mut self, data: T) {
        let new_ptr = Box::into_raw(Box::new(ForwardNode::new(data, ptr::null_mut())));
        if !self.head.is_null() {
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
            let new_ptr = Box::into_raw(Box::new(ForwardNode::new(data, (*node).next)));
            (*node).next = new_ptr;
            if (*new_ptr).next.is_null() {
                self.tail = new_ptr;
            }
        }
    }

    pub fn insertBefore(&mut self, data: T, pos_data: &T) {
        let node = self.find(pos_data);
        if node.is_null() {
            panic!("position element not found");
        }
        if node == self.head {
            self.pushFront(data);
            return;
        }
        let mut cur = self.head;
        unsafe {
            while !cur.is_null() && (*cur).next != node {
                cur = (*cur).next;
            }
            let new_ptr = Box::into_raw(Box::new(ForwardNode::new(data, node)));
            (*cur).next = new_ptr;
        }
    }

    pub fn popFront(&mut self) {
        if self.head.is_null() {
            panic!("list is empty");
        }
        unsafe {
            let del = Box::from_raw(self.head);
            self.head = (*del).next;
            if self.head.is_null() {
                self.tail = ptr::null_mut();
            }
        }
    }

    pub fn popBack(&mut self) {
        if self.head.is_null() {
            panic!("list is empty");
        }
        if self.head == self.tail {
            unsafe { drop(Box::from_raw(self.head)); }
            self.head = ptr::null_mut();
            self.tail = ptr::null_mut();
            return;
        }
        let mut prev = self.head;
        unsafe {
            while (*prev).next != self.tail {
                prev = (*prev).next;
            }
            drop(Box::from_raw(self.tail));
            self.tail = prev;
            (*self.tail).next = ptr::null_mut();
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
            if del.next.is_null() {
                self.tail = node;
            }
        }
    }

    pub fn removeBefore(&mut self, pos_data: &T) {
        let node = self.find(pos_data);
        if node.is_null() {
            panic!("position element not found");
        }
        if node == self.head {
            panic!("no node before this one");
        }
        unsafe {
            if (*self.head).next == node {
                let del = Box::from_raw(self.head);
                self.head = node;
                drop(del);
                return;
            }
            let mut cur = self.head;
            while !(*cur).next.is_null() && (*(*cur).next).next != node {
                cur = (*cur).next;
            }
            let del = Box::from_raw((*cur).next);
            (*cur).next = node;
            drop(del);
        }
    }

    pub fn deleteValue(&mut self, data: &T) -> bool {
        if self.head.is_null() { return false; }
        unsafe {
            if (*self.head).data == *data {
                self.popFront();
                return true;
            }
            let mut cur = self.head;
            while !(*cur).next.is_null() && (*(*cur).next).data != *data {
                cur = (*cur).next;
            }
            if (*cur).next.is_null() { return false; }
            let del = Box::from_raw((*cur).next);
            (*cur).next = del.next;
            if del.next.is_null() {
                self.tail = cur;
            }
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

    pub fn find(&self, data: &T) -> *mut ForwardNode<T> {
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

impl<T> Drop for ForwardList<T> {
    fn drop(&mut self) {
        while !self.head.is_null() {
            unsafe {
                let del = Box::from_raw(self.head);
                self.head = (*del).next;
            }
        }
    }
}