use std::alloc::{alloc, dealloc, Layout};
use std::fmt::Display;
use std::io::Write;
use std::ptr;

pub struct ArrayList<T> {
    pub data: *mut T,
    pub size: usize,
    pub capacity: usize,
}

impl<T> ArrayList<T> {
    pub fn new() -> Self {
        ArrayList {
            data: ptr::null_mut(),
            size: 0,
            capacity: 0,
        }
    }

    fn layout(capacity: usize) -> Layout {
        Layout::array::<T>(capacity).unwrap()
    }

    pub fn pushBack(&mut self, value: T) {
        if self.size < self.capacity {
            unsafe {
                self.data.add(self.size).write(value);
            }
            self.size += 1;
        } else {
            let new_cap = self.capacity + 1;
            let new_layout = Self::layout(new_cap);
            let new_data = unsafe { alloc(new_layout) as *mut T };
            if new_data.is_null() {
                panic!("allocation failed");
            }

            unsafe {
                ptr::copy_nonoverlapping(self.data, new_data, self.size);
                if self.capacity > 0 {
                    dealloc(self.data as *mut u8, Self::layout(self.capacity));
                }
                new_data.add(self.size).write(value);
            }

            self.data = new_data;
            self.capacity = new_cap;
            self.size += 1;
        }
    }

    pub fn insert(&mut self, value: T, index: usize) {
        if index > self.size {
            panic!("index out of range");
        } else if index == self.size {
            self.pushBack(value);
            return;
        }

        if self.size < self.capacity {
            unsafe {
                ptr::copy(
                    self.data.add(index),
                    self.data.add(index + 1),
                    self.size - index,
                );
                self.data.add(index).write(value);
            }
            self.size += 1;
        } else {
            let new_cap = self.capacity + 1;
            let new_layout = Self::layout(new_cap);
            let new_data = unsafe { alloc(new_layout) as *mut T };
            if new_data.is_null() {
                panic!("allocation failed");
            }

            unsafe {
                ptr::copy_nonoverlapping(self.data, new_data, index);
                new_data.add(index).write(value);
                ptr::copy_nonoverlapping(
                    self.data.add(index),
                    new_data.add(index + 1),
                    self.size - index,
                );
                if self.capacity > 0 {
                    dealloc(self.data as *mut u8, Self::layout(self.capacity));
                }
            }

            self.data = new_data;
            self.capacity = new_cap;
            self.size += 1;
        }
    }

    pub fn get(&self, index: usize) -> &T {
        if index >= self.size {
            panic!("index out of range");
        }
        unsafe { &*self.data.add(index) }
    }

    pub fn set(&mut self, value: T, index: usize) {
        if index >= self.size {
            panic!("index out of range");
        }
        unsafe {
            *self.data.add(index) = value;
        }
    }

    pub fn remove(&mut self, index: usize) {
        if index >= self.size {
            panic!("index out of range");
        }
        unsafe {
            let ptr = self.data.add(index);
            let removed = ptr::read(ptr);
            ptr::copy(ptr.add(1), ptr, self.size - index - 1);
            drop(removed);
        }
        self.size -= 1;
    }

    pub fn getSize(&self) -> usize {
        self.size
    }

    pub fn print(&self)
    where
        T: Display,
    {
        for i in 0..self.size {
            let item = unsafe { &*self.data.add(i) };
            print!("{} ", item);
        }
        println!();
    }

    pub fn save(&self, os: &mut impl Write)
    where
        T: Display,
    {
        for i in 0..self.size {
            let item = unsafe { &*self.data.add(i) };
            writeln!(os, "{}", item).unwrap();
        }
    }
}

impl<T> Drop for ArrayList<T> {
    fn drop(&mut self) {
        for i in 0..self.size {
            unsafe {
                ptr::drop_in_place(self.data.add(i));
            }
        }
        if self.capacity > 0 {
            unsafe {
                dealloc(self.data as *mut u8, Self::layout(self.capacity));
            }
        }
    }
}