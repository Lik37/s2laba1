#include "database.h"
#include "io.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string filePath, query;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) filePath = argv[++i];
        else if (arg == "--query" && i + 1 < argc) query = argv[++i];
    }

    if (filePath.empty() || query.empty()) {
        std::cerr << "Usage: ./dbms --file <path> --query '<command>'\n";
        return 1;
    }

    Database db;
    load(filePath, db);

    auto tokens = parse(query);
    if (tokens.empty()) {
        std::cerr << "Empty query\n";
        return 1;
    }

    Command cmd = toCommand(tokens[0]);
    std::string name = tokens.size() > 1 ? tokens[1] : "";
    std::string value = tokens.size() > 2 ? tokens[2] : "";
    std::string index = tokens.size() > 3 ? tokens[3] : "";

    switch (cmd) {
        
        // array
        case Command::MPUSH:
            db.arrays[name].pushBack(value);
            std::cout << value << "\n";
            break;
        case Command::MDEL:
            db.arrays[name].remove(std::stoi(value));
            break;
        case Command::MGET:
            std::cout << db.arrays[name][std::stoi(value)] << "\n";
            break;
        case Command::MINSERT:
            db.arrays[name].insert(value, std::stoi(index));
            break;
        case Command::MSET: // MSET имя значение индекс
            db.arrays[name][std::stoi(index)] = value;
            break;
        case Command::MSIZE:
            std::cout << db.arrays[name].getSize() << "\n";
            break;

        // forw-list
        case Command::FPUSH:
            db.forwardLists[name].pushBack(value);
            std::cout << value << "\n";
            break;
        case Command::FPUSHFRONT:
            db.forwardLists[name].pushFront(value);
            std::cout << value << "\n";
            break;
        case Command::FPOPFRONT:
            db.forwardLists[name].popFront();
            break;
        case Command::FPOPBACK:
            db.forwardLists[name].popBack();
            break;
        case Command::FINSERTAFTER:  // FINSERTAFTER имя значение индекс
            db.forwardLists[name].insertAfter(value, index);
            break;
        case Command::FINSERTBEFORE: // FINSERTBEFORE имя значение индекс
            db.forwardLists[name].insertBefore(value, index);
            break;
        case Command::FREMOVEAFTER:
            db.forwardLists[name].removeAfter(index);
            break;
        case Command::FREMOVEBEFORE:
            db.forwardLists[name].removeBefore(index);
            break;
        case Command::FDEL:
            db.forwardLists[name].deleteValue(value);
            break;
        case Command::FGET:
            std::cout << db.forwardLists[name].front() << "\n";
            break;
        case Command::FGETBACK:
            std::cout << db.forwardLists[name].back() << "\n";
            break;
        case Command::FGETIDX:
            std::cout << db.forwardLists[name][std::stoi(value)] << "\n";
            break;
        case Command::FFIND:
            if (db.forwardLists[name].find(value) != nullptr)
                std::cout << "TRUE\n";
            else
                std::cout << "FALSE\n";
            break;

        // doubly-list
        case Command::LPUSH:
            db.doublyLists[name].pushBack(value);
            std::cout << value << "\n";
            break;
        case Command::LPUSHFRONT:
            db.doublyLists[name].pushFront(value);
            std::cout << value << "\n";
            break;
        case Command::LPOPFRONT:
            db.doublyLists[name].popFront();
            break;
        case Command::LPOPBACK:
            db.doublyLists[name].popBack();
            break;
        case Command::LINSERTAFTER:  // LINSERTAFTER имя значение индекс
            db.doublyLists[name].insertAfter(value, index);
            break;
        case Command::LINSERTBEFORE: // LINSERTBEFORE имя значение индекс
            db.doublyLists[name].insertBefore(value, index);
            break;
        case Command::LREMOVEAFTER:
            db.doublyLists[name].removeAfter(value);
            break;
        case Command::LREMOVEBEFORE:
            db.doublyLists[name].removeBefore(value);
            break;
        case Command::LDEL:
            db.doublyLists[name].deleteValue(value);
            break;
        case Command::LGET:
            std::cout << db.doublyLists[name].front() << "\n";
            break;
        case Command::LGETBACK:
            std::cout << db.doublyLists[name].back() << "\n";
            break;
        case Command::LGETIDX:
            std::cout << db.doublyLists[name][std::stoi(value)] << "\n";
            break;
        case Command::LFIND:
            if (db.doublyLists[name].find(value) != nullptr)
                std::cout << "TRUE\n";
            else
                std::cout << "FALSE\n";
            break;

        // queue
        case Command::QPUSH:
            db.queues[name].push(value);
            std::cout << value << "\n";
            break;
        case Command::QPOP:
            std::cout << db.queues[name].pop() << "\n";
            break;
        case Command::QGET:
            std::cout << db.queues[name].front() << "\n";
            break;
        case Command::QBACK:
            std::cout << db.queues[name].back() << "\n";
            break;

        // stack
        case Command::SPUSH:
            db.stacks[name].push(value);
            std::cout << value << "\n";
            break;
        case Command::SPOP:
            std::cout << db.stacks[name].pop() << "\n";
            break;
        case Command::SGET:
            std::cout << db.stacks[name].front() << "\n";
            break;
        
        // avl-tree
        case Command::TINSERT:
            db.trees[name].add(value);
            std::cout << value << "\n";
            break;
        case Command::TDEL:
            db.trees[name].remove(value);
            break;
        case Command::TGET:
            if (db.trees[name].find(value))
                std::cout << "TRUE\n";
            else
                std::cout << "FALSE\n";
            break;
        case Command::TBFS:
        {
            Queue<std::string> q = db.trees[name].bfs();
            while (!q.isEmpty()) {
                std::cout << q.front() << " ";
                q.pop();
            }
            std::cout << "\n";
            break;
        }

        // other
        case Command::PRINT:
            if (name.empty()) {
                std::cerr << "PRINT needs a name\n";
                break;
            }
            if (db.stacks.count(name))
                db.stacks[name].print();
            else if (db.queues.count(name)) 
                db.queues[name].print();
            else if (db.forwardLists.count(name)) 
                db.forwardLists[name].print();
            else if (db.doublyLists.count(name)) 
                db.doublyLists[name].print();
            else if (db.arrays.count(name)) 
                db.arrays[name].print();
            else if (db.trees.count(name)) 
                db.trees[name].print();
            break;

        case Command::UNKNOWN:
            std::cerr << "Unknown command: " << tokens[0] << "\n";
            return 1;
    }

    save(filePath, db);
    return 0;
}