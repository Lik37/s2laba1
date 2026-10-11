#include "io.h"

#include <fstream>
#include <sstream>

std::vector<std::string> parse(const std::string& query) {
    std::vector<std::string> tokens;
    std::istringstream iss(query);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

Command toCommand(const std::string& token) {
    if (token.empty()) 
        return Command::UNKNOWN;

    // array
    else if (token[0] == 'M') {
        if (token == "MPUSH")
            return Command::MPUSH;
        if (token == "MDEL")
            return Command::MDEL;
        if (token == "MGET")
            return Command::MGET;
        if (token == "MINSERT")
            return Command::MINSERT;
        if (token == "MSET")
            return Command::MSET;
        if (token == "MSIZE")
            return Command::MSIZE;
    }
    // forward
    else if (token[0] == 'F') {
        if (token == "FPUSH")         
            return Command::FPUSH;
        if (token == "FPUSHFRONT")    
            return Command::FPUSHFRONT;
        if (token == "FPOPFRONT")     
            return Command::FPOPFRONT;
        if (token == "FPOPBACK")      
            return Command::FPOPBACK;
        if (token == "FINSERTAFTER")  
            return Command::FINSERTAFTER;
        if (token == "FINSERTBEFORE") 
            return Command::FINSERTBEFORE;
        if (token == "FREMOVEAFTER")  
            return Command::FREMOVEAFTER;
        if (token == "FREMOVEBEFORE") 
            return Command::FREMOVEBEFORE;
        if (token == "FDEL")          
            return Command::FDEL;
        if (token == "FGET")          
            return Command::FGET;
        if (token == "FGETBACK")      
            return Command::FGETBACK;
        if (token == "FGETIDX")       
            return Command::FGETIDX;
        if (token == "FFIND")         
            return Command::FFIND;
    }
    // doubly
    else if (token[0] == 'L') {
        if (token == "LPUSH")         
            return Command::LPUSH;
        if (token == "LPUSHFRONT")    
            return Command::LPUSHFRONT;
        if (token == "LPOPFRONT")     
            return Command::LPOPFRONT;
        if (token == "LPOPBACK")      
            return Command::LPOPBACK;
        if (token == "LINSERTAFTER")  
            return Command::LINSERTAFTER;
        if (token == "LINSERTBEFORE") 
            return Command::LINSERTBEFORE;
        if (token == "LREMOVEAFTER")
            return Command::LREMOVEAFTER;
        if (token == "LREMOVEBEFORE") 
            return Command::LREMOVEBEFORE;
        if (token == "LDEL")          
            return Command::LDEL;
        if (token == "LGET")          
            return Command::LGET;
        if (token == "LGETBACK")      
            return Command::LGETBACK;
        if (token == "LGETIDX")       
            return Command::LGETIDX;
        if (token == "LFIND")         
            return Command::LFIND;
    }
    // queue
    else if (token[0] == 'Q') {
        if (token == "QPUSH")
            return Command::QPUSH;
        if (token == "QPOP")
            return Command::QPOP;
        if (token == "QGET")
            return Command::QGET;
        if (token == "QBACK")
            return Command::QBACK;
    }
    // stack
    else if (token[0] == 'S') {
        if (token == "SPUSH")
            return Command::SPUSH;
        if (token == "SPOP")
            return Command::SPOP;
        if (token == "SGET")
            return Command::SGET;
    } 
    // avl-tree
    else if (token[0] == 'T') {
        if (token == "TINSERT") 
            return Command::TINSERT;
        if (token == "TDEL")    
            return Command::TDEL;
        if (token == "TGET")    
            return Command::TGET;
        if (token == "TBFS")    
            return Command::TBFS;
    }

    // other
    if (token == "PRINT")
        return Command::PRINT;

    return Command::UNKNOWN;
}

void load(const std::string& filePath, Database& db) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string type, name;
        iss >> type >> name;

        std::vector<std::string> values;
        while (std::getline(file, line)) {
            if (line == "END") break;
            if (!line.empty()) values.push_back(line);
        }

        if (type == "STACK") {
            for (auto it = values.rbegin(); it != values.rend(); ++it)
                db.stacks[name].push(*it);
        } else if (type == "QUEUE") {
            for (auto& v : values) db.queues[name].push(v);
        } else if (type == "FLIST") {
            for (auto& v : values) db.forwardLists[name].pushBack(v);
        } else if (type == "DLIST") {
            for (auto& v : values) db.doublyLists[name].pushBack(v);
        } else if (type == "ARRAY") {
            for (auto& v : values) db.arrays[name].pushBack(v);
        } else if (type == "TREE") {
            for (auto& v : values) db.trees[name].add(v);
        }
    }
}

void save(const std::string& filePath, const Database& db) {
    std::ofstream file(filePath);

    for (auto& [name, s] : db.stacks) {
        file << "STACK " << name << "\n";
        s.save(file);
        file << "END\n\n";
    }
    for (auto& [name, q] : db.queues) {
        file << "QUEUE " << name << "\n";
        q.save(file);
        file << "END\n\n";
    }
    for (auto& [name, l] : db.forwardLists) {
        file << "FLIST " << name << "\n";
        l.save(file);
        file << "END\n\n";
    }
    for (auto& [name, l] : db.doublyLists) {
        file << "DLIST " << name << "\n";
        l.save(file);
        file << "END\n\n";
    }
    for (auto& [name, a] : db.arrays) {
        file << "ARRAY " << name << "\n";
        a.save(file);
        file << "END\n\n";
    }
    for (auto& [name, t] : db.trees) {
        file << "TREE " << name << "\n";
        t.save(file);
        file << "END\n\n";
    }
}