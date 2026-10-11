// Очередь.
use crate::structs::node::ForwardNode;
use std::io::Write;
use std::ptr;

pub struct Queue<T> {
    pub head: *mut ForwardNode<T>,
    pub tail: *mut ForwardNode<T>,
}

impl<T> Queue<T> {
    pub fn new() -> Self {
        Self { head: ptr::null_mut(), tail: ptr::null_mut() }
    }

    pub fn push(&mut self, data: T) {
        let new_node = ForwardNode::new(data, ptr::null_mut());
        let new_ptr = Box::into_raw(Box::new(new_node));

        if !self.tail.is_null() {
            unsafe { (*self.tail).next = new_ptr; }
        } else {
            self.head = new_ptr;
        }
        self.tail = new_ptr;
    }

    pub fn pop(&mut self) -> T {
        if self.head.is_null() {
            panic!("queue is empty");
        }
        unsafe {
            let del_node = Box::from_raw(self.head);
            self.head = del_node.next;
            if self.head.is_null() {
                self.tail = ptr::null_mut();
            }
            del_node.data
        }
    }

    pub fn front(&self) -> &T {
        if self.head.is_null() {
            panic!("queue is empty");
        }
        unsafe { &(*self.head).data }
    }

    pub fn back(&self) -> &T {
        if self.tail.is_null() {
            panic!("queue is empty");
        }
        unsafe { &(*self.tail).data }
    }

    pub fn isEmpty(&self) -> bool {
        self.head.is_null()
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

impl<T> Drop for Queue<T> {
    fn drop(&mut self) {
        while !self.head.is_null() {
            unsafe {
                let del = Box::from_raw(self.head);
                self.head = (*del).next;
            }
        }
    }
}