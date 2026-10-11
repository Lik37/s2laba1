use crate::database::{Command, Database};
use std::fs::File;
use std::io::{BufRead, BufReader, Write};

use crate::structs::stack::Stack;
use crate::structs::queue::Queue;
use crate::structs::forward_list::ForwardList;
use crate::structs::doubly_list::DoublyList;
use crate::structs::array_list::ArrayList;

pub fn parse(query: &str) -> Vec<String> {
    query.split_whitespace().map(|s| s.to_string()).collect()
}

pub fn to_command(token: &str) -> Command {
    if token.is_empty() { return Command::UNKNOWN; }
    let b = token.as_bytes()[0];
    match b {
        b'M' => match token {
            "MPUSH" => Command::MPUSH,
            "MDEL" => Command::MDEL,
            "MGET" => Command::MGET,
            "MINSERT" => Command::MINSERT,
            "MSET" => Command::MSET,
            "MSIZE" => Command::MSIZE,
            _ => Command::UNKNOWN,
        },
        b'F' => match token {
            "FPUSH" => Command::FPUSH,
            "FPUSHFRONT" => Command::FPUSHFRONT,
            "FPOPFRONT" => Command::FPOPFRONT,
            "FPOPBACK" => Command::FPOPBACK,
            "FINSERTAFTER" => Command::FINSERTAFTER,
            "FINSERTBEFORE" => Command::FINSERTBEFORE,
            "FREMOVEAFTER" => Command::FREMOVEAFTER,
            "FREMOVEBEFORE" => Command::FREMOVEBEFORE,
            "FDEL" => Command::FDEL,
            "FGET" => Command::FGET,
            "FGETBACK" => Command::FGETBACK,
            "FGETIDX" => Command::FGETIDX,
            "FFIND" => Command::FFIND,
            _ => Command::UNKNOWN,
        },
        b'L' => match token {
            "LPUSH" => Command::LPUSH,
            "LPUSHFRONT" => Command::LPUSHFRONT,
            "LPOPFRONT" => Command::LPOPFRONT,
            "LPOPBACK" => Command::LPOPBACK,
            "LINSERTAFTER" => Command::LINSERTAFTER,
            "LINSERTBEFORE" => Command::LINSERTBEFORE,
            "LREMOVEAFTER" => Command::LREMOVEAFTER,
            "LREMOVEBEFORE" => Command::LREMOVEBEFORE,
            "LDEL" => Command::LDEL,
            "LGET" => Command::LGET,
            "LGETBACK" => Command::LGETBACK,
            "LGETIDX" => Command::LGETIDX,
            "LFIND" => Command::LFIND,
            _ => Command::UNKNOWN,
        },
        b'Q' => match token {
            "QPUSH" => Command::QPUSH,
            "QPOP" => Command::QPOP,
            "QGET" => Command::QGET,
            "QBACK" => Command::QBACK,
            _ => Command::UNKNOWN,
        },
        b'S' => match token {
            "SPUSH" => Command::SPUSH,
            "SPOP" => Command::SPOP,
            "SGET" => Command::SGET,
            _ => Command::UNKNOWN,
        },
        _ => {
            if token == "PRINT" { Command::PRINT } else { Command::UNKNOWN }
        }
    }
}

pub fn load(file_path: &str, db: &mut Database) {
    let file = match File::open(file_path) {
        Ok(f) => f,
        Err(_) => return,
    };
    let reader = BufReader::new(file);
    let mut lines = reader.lines();

    while let Some(Ok(line)) = lines.next() {
        if line.is_empty() { continue; }

        let mut parts = line.split_whitespace();
        let typ = parts.next().unwrap_or("");
        let name = parts.next().unwrap_or("").to_string();

        let mut values: Vec<String> = Vec::new();
        while let Some(Ok(l)) = lines.next() {
            if l == "END" { break; }
            if !l.is_empty() { values.push(l); }
        }

        match typ {
            "STACK" => {
                let s = db.stacks.entry(name).or_insert_with(Stack::new);
                for v in values.iter().rev() {
                    s.push(v.clone());
                }
            }
            "QUEUE" => {
                let q = db.queues.entry(name).or_insert_with(Queue::new);
                for v in values {
                    q.push(v);
                }
            }
            "FLIST" => {
                let l = db.forward_lists.entry(name).or_insert_with(ForwardList::new);
                for v in values {
                    l.pushBack(v);
                }
            }
            "DLIST" => {
                let l = db.doubly_lists.entry(name).or_insert_with(DoublyList::new);
                for v in values {
                    l.pushBack(v);
                }
            }
            "ARRAY" => {
                let a = db.arrays.entry(name).or_insert_with(ArrayList::new);
                for v in values {
                    a.pushBack(v);
                }
            }
            _ => {}
        }
    }
}

pub fn save(file_path: &str, db: &Database) {
    let mut file = File::create(file_path).unwrap();

    for (name, s) in &db.stacks {
        writeln!(file, "STACK {}", name).unwrap();
        s.save(&mut file);
        writeln!(file, "END\n").unwrap();
    }
    for (name, q) in &db.queues {
        writeln!(file, "QUEUE {}", name).unwrap();
        q.save(&mut file);
        writeln!(file, "END\n").unwrap();
    }
    for (name, l) in &db.forward_lists {
        writeln!(file, "FLIST {}", name).unwrap();
        l.save(&mut file);
        writeln!(file, "END\n").unwrap();
    }
    for (name, l) in &db.doubly_lists {
        writeln!(file, "DLIST {}", name).unwrap();
        l.save(&mut file);
        writeln!(file, "END\n").unwrap();
    }
    for (name, a) in &db.arrays {
        writeln!(file, "ARRAY {}", name).unwrap();
        a.save(&mut file);
        writeln!(file, "END\n").unwrap();
    }
}