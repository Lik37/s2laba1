#pragma once

#include "structs/stack.h"
#include "structs/queue.h"
#include "structs/forward_list.h"
#include "structs/doubly_list.h"
#include "structs/array_list.h"
#include "structs/avl_tree.h"

#include <map>
#include <string>

enum class Command {
    MPUSH, MDEL, MGET, MINSERT, MSET, MSIZE,

    FINSERTAFTER, FINSERTBEFORE, FREMOVEAFTER, FREMOVEBEFORE,
    FPUSH, FPUSHFRONT, 
    FPOPFRONT, FPOPBACK, FDEL,
    FGET, FGETBACK, FGETIDX, FFIND,

    LINSERTAFTER, LINSERTBEFORE, LREMOVEAFTER, LREMOVEBEFORE,
    LPUSH, LPUSHFRONT, 
    LPOPFRONT, LPOPBACK, LDEL,
    LGET, LGETBACK, LGETIDX, LFIND,

    QPUSH, QPOP, QGET, QBACK,

    SPUSH, SPOP, SGET,

    TINSERT, TDEL, TGET, TBFS,

    PRINT,

    UNKNOWN
};


struct Database {
    std::map<std::string, Stack<std::string>> stacks;
    std::map<std::string, Queue<std::string>> queues;
    std::map<std::string, ForwardList<std::string>> forwardLists;
    std::map<std::string, DoublyList<std::string>> doublyLists;
    std::map<std::string, ArrayList<std::string>> arrays;
    std::map<std::string, AVLTree<std::string>> trees;
};