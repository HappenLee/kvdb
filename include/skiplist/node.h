//
// Created by happen on 9/15/18.
//

#ifndef LDB_NODE_H
#define LDB_NODE_H

#include <cstring>
#include <memory>
#include <vector>

#include "common.h"

class Node {
public:
    Node(const std::string &key, const char* value, uint64_t value_size, int level);
    ~Node();
    Node(const Node &node) = delete;
    Node& operator=(const Node &node) = delete;
    Node(const Node &&node) = delete;
    Node& operator=(const Node &&node) = delete;
private:
    Node();
    std::string key;
    std::shared_ptr<char> value;
    uint64_t value_size;
    std::vector<Node*> forward;

    friend class SkipList;
};
#endif //LDB_NODE_H










