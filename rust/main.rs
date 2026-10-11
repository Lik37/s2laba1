mod structs;
mod database;
mod io;

use crate::database::{Command, Database};
use crate::structs::array_list::ArrayList;
use crate::structs::stack::Stack;
use crate::structs::queue::Queue;
use crate::structs::forward_list::ForwardList;
use crate::structs::doubly_list::DoublyList;

use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();

    let mut file_path = String::new();
    let mut query = String::new();

    let mut i = 1;
    while i < args.len() {
        if args[i] == "--file" && i + 1 < args.len() {
            file_path = args[i + 1].clone();
            i += 2;
        } else if args[i] == "--query" && i + 1 < args.len() {
            query = args[i + 1].clone();
            i += 2;
        } else {
            i += 1;
        }
    }

    if file_path.is_empty() || query.is_empty() {
        eprintln!("Usage: ./dbms --file <path> --query '<command>'");
        std::process::exit(1);
    }

    let mut db = Database::new();
    io::load(&file_path, &mut db);

    let tokens = io::parse(&query);
    if tokens.is_empty() {
        eprintln!("Empty query");
        std::process::exit(1);
    }

    let cmd = io::to_command(&tokens[0]);
    let name = tokens.get(1).cloned().unwrap_or_default();
    let value = tokens.get(2).cloned().unwrap_or_default();
    let index = tokens.get(3).cloned().unwrap_or_default();

    match cmd {
        // array
        Command::MPUSH => {
            db.arrays.entry(name).or_insert_with(ArrayList::new).pushBack(value.clone());
            println!("{}", value);
        }
        Command::MDEL => {
            db.arrays.get_mut(&name).unwrap().remove(value.parse().unwrap());
        }
        Command::MGET => {
            println!("{}", db.arrays.get(&name).unwrap().get(value.parse().unwrap()));
        }
        Command::MINSERT => {
            db.arrays.get_mut(&name).unwrap().insert(value, index.parse().unwrap());
        }
        Command::MSET => {
            db.arrays.get_mut(&name).unwrap().set(value, index.parse().unwrap());
        }
        Command::MSIZE => {
            println!("{}", db.arrays.get(&name).unwrap().getSize());
        }

        // forward list
        Command::FPUSH => {
            db.forward_lists.entry(name).or_insert_with(ForwardList::new).pushBack(value.clone());
            println!("{}", value);
        }
        Command::FPUSHFRONT => {
            db.forward_lists.entry(name).or_insert_with(ForwardList::new).pushFront(value.clone());
            println!("{}", value);
        }
        Command::FPOPFRONT => {
            db.forward_lists.get_mut(&name).unwrap().popFront();
        }
        Command::FPOPBACK => {
            db.forward_lists.get_mut(&name).unwrap().popBack();
        }
        Command::FINSERTAFTER => {
            db.forward_lists.get_mut(&name).unwrap().insertAfter(value, &index);
        }
        Command::FINSERTBEFORE => {
            db.forward_lists.get_mut(&name).unwrap().insertBefore(value, &index);
        }
        Command::FREMOVEAFTER => {
            db.forward_lists.get_mut(&name).unwrap().removeAfter(&value);
        }
        Command::FREMOVEBEFORE => {
            db.forward_lists.get_mut(&name).unwrap().removeBefore(&value);
        }
        Command::FDEL => {
            db.forward_lists.get_mut(&name).unwrap().deleteValue(&value);
        }
        Command::FGET => {
            println!("{}", db.forward_lists.get(&name).unwrap().front());
        }
        Command::FGETBACK => {
            println!("{}", db.forward_lists.get(&name).unwrap().back());
        }
        Command::FGETIDX => {
            println!("{}", db.forward_lists.get(&name).unwrap().get(value.parse().unwrap()));
        }
        Command::FFIND => {
            let found = !db.forward_lists.get(&name).unwrap().find(&value).is_null();
            println!("{}", if found { "TRUE" } else { "FALSE" });
        }

        // doubly list
        Command::LPUSH => {
            db.doubly_lists.entry(name).or_insert_with(DoublyList::new).pushBack(value.clone());
            println!("{}", value);
        }
        Command::LPUSHFRONT => {
            db.doubly_lists.entry(name).or_insert_with(DoublyList::new).pushFront(value.clone());
            println!("{}", value);
        }
        Command::LPOPFRONT => {
            db.doubly_lists.get_mut(&name).unwrap().popFront();
        }
        Command::LPOPBACK => {
            db.doubly_lists.get_mut(&name).unwrap().popBack();
        }
        Command::LINSERTAFTER => {
            db.doubly_lists.get_mut(&name).unwrap().insertAfter(value, &index);
        }
        Command::LINSERTBEFORE => {
            db.doubly_lists.get_mut(&name).unwrap().insertBefore(value, &index);
        }
        Command::LREMOVEAFTER => {
            db.doubly_lists.get_mut(&name).unwrap().removeAfter(&value);
        }
        Command::LREMOVEBEFORE => {
            db.doubly_lists.get_mut(&name).unwrap().removeBefore(&value);
        }
        Command::LDEL => {
            db.doubly_lists.get_mut(&name).unwrap().deleteValue(&value);
        }
        Command::LGET => {
            println!("{}", db.doubly_lists.get(&name).unwrap().front());
        }
        Command::LGETBACK => {
            println!("{}", db.doubly_lists.get(&name).unwrap().back());
        }
        Command::LGETIDX => {
            println!("{}", db.doubly_lists.get(&name).unwrap().get(value.parse().unwrap()));
        }
        Command::LFIND => {
            let found = !db.doubly_lists.get(&name).unwrap().find(&value).is_null();
            println!("{}", if found { "TRUE" } else { "FALSE" });
        }

        // queue
        Command::QPUSH => {
            db.queues.entry(name).or_insert_with(Queue::new).push(value.clone());
            println!("{}", value);
        }
        Command::QPOP => {
            println!("{}", db.queues.get_mut(&name).unwrap().pop());
        }
        Command::QGET => {
            println!("{}", db.queues.get(&name).unwrap().front());
        }
        Command::QBACK => {
            println!("{}", db.queues.get(&name).unwrap().back());
        }

        // stack
        Command::SPUSH => {
            db.stacks.entry(name).or_insert_with(Stack::new).push(value.clone());
            println!("{}", value);
        }
        Command::SPOP => {
            println!("{}", db.stacks.get_mut(&name).unwrap().pop());
        }
        Command::SGET => {
            println!("{}", db.stacks.get(&name).unwrap().front());
        }

        // other
        Command::PRINT => {
            if name.is_empty() {
                eprintln!("PRINT needs a name");
                return;
            }
            if let Some(s) = db.stacks.get(&name) {
                s.print();
            } else if let Some(q) = db.queues.get(&name) {
                q.print();
            } else if let Some(l) = db.forward_lists.get(&name) {
                l.print();
            } else if let Some(l) = db.doubly_lists.get(&name) {
                l.print();
            } else if let Some(a) = db.arrays.get(&name) {
                a.print();
            }
        }

        Command::UNKNOWN => {
            eprintln!("Unknown command: {}", tokens[0]);
            std::process::exit(1);
        }
    }

    io::save(&file_path, &db);
}