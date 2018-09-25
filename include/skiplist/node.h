//
// Created by happen on 9/15/18.
//

#ifndef LDB_NODE_H
#define LDB_NODE_H

#include <vector>
#include <memory>
#include <cstring>

#include "common.h"

class Node {
public:
    Node(std::string key, const char* value, uint64_t value_size, int level) {
        this->key = key;
        this->value_size = value_size;
        char* new_value = new char[value_size];
        if (new_value != nullptr) {
            this->value = std::shared_ptr<char>(new_value, [](char* value_ptr) { SAFE_DELETE_ARR(value_ptr); });
            memcpy(new_value, value, value_size);
            forward = std::vector<Node *>(level);
        }
    }


    ~Node() {
        this->value.reset();
    }

private:
    std::string key;
    std::shared_ptr<char> value;
    uint64_t value_size;
    std::vector<Node*> forward;

    friend class SkipList;
};
#endif //LDB_NODE_H










