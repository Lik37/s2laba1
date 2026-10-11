use crate::structs::stack::Stack;
use crate::structs::queue::Queue;
use crate::structs::forward_list::ForwardList;
use crate::structs::doubly_list::DoublyList;
use crate::structs::array_list::ArrayList;
use std::collections::HashMap;

#[derive(Clone, Copy, PartialEq)]
pub enum Command {
    MPUSH, MDEL, MGET, MINSERT, MSET, MSIZE,

    FINSERTAFTER, FINSERTBEFORE, FREMOVEAFTER, FREMOVEBEFORE,
    FPUSH, FPUSHFRONT, FPOPFRONT, FPOPBACK, FDEL,
    FGET, FGETBACK, FGETIDX, FFIND,

    LINSERTAFTER, LINSERTBEFORE, LREMOVEAFTER, LREMOVEBEFORE,
    LPUSH, LPUSHFRONT, LPOPFRONT, LPOPBACK, LDEL,
    LGET, LGETBACK, LGETIDX, LFIND,

    QPUSH, QPOP, QGET, QBACK,

    SPUSH, SPOP, SGET,

    PRINT,
    UNKNOWN,
}

pub struct Database {
    pub stacks: HashMap<String, Stack<String>>,
    pub queues: HashMap<String, Queue<String>>,
    pub forward_lists: HashMap<String, ForwardList<String>>,
    pub doubly_lists: HashMap<String, DoublyList<String>>,
    pub arrays: HashMap<String, ArrayList<String>>,
}

impl Database {
    pub fn new() -> Self {
        Database {
            stacks: HashMap::new(),
            queues: HashMap::new(),
            forward_lists: HashMap::new(),
            doubly_lists: HashMap::new(),
            arrays: HashMap::new(),
        }
    }
}