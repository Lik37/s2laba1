// Стек.
use crate::structs::node::ForwardNode;
use std::io::Write;
use std::ptr;

pub struct Stack<T> {
    pub head: *mut ForwardNode<T>,
}

impl<T> Stack<T> {
    pub fn new() -> Self {
        Self { head: ptr::null_mut() }
    }

    pub fn push(&mut self, data: T) {
        let new_node = ForwardNode::new(data, self.head);
        self.head = Box::into_raw(Box::new(new_node));
    }

    pub fn pop(&mut self) -> T {
        if self.head.is_null() {
            panic!("stack is empty");
        }
        unsafe {
            let del_node = Box::from_raw(self.head);
            self.head = del_node.next;
            del_node.data
        }
    }

    pub fn front(&self) -> &T {
        if self.head.is_null() {
            panic!("stack is empty");
        }
        unsafe { &(*self.head).data }
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

impl<T> Drop for Stack<T> {
    fn drop(&mut self) {
        while !self.head.is_null() {
            unsafe {
                let del = Box::from_raw(self.head);
                self.head = (*del).next;
            }
        }
    }
}