pub struct ForwardNode<T> {
    pub data: T,
    pub next: *mut ForwardNode<T>,
}

impl<T> ForwardNode<T> {
    pub fn new(data: T, next: *mut ForwardNode<T>) -> Self {
        ForwardNode { data, next }
    }
}

pub struct DoublyNode<T> {
    pub data: T,
    pub next: *mut DoublyNode<T>,
    pub prev: *mut DoublyNode<T>,
}

impl<T> DoublyNode<T> {
    pub fn new(data: T, next: *mut DoublyNode<T>, prev: *mut DoublyNode<T>) -> Self {
        DoublyNode { data, next, prev }
    }
}