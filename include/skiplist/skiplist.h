//
// Created by happen on 9/15/18.
//
#include <cmath>
#include <shared_mutex>
#include "node.h"
#include "common.h"

#ifndef LDB_SKIPLIST_H
#define LDB_SKIPLIST_H

struct Handle{
    std::shared_ptr<char> value;
    uint64_t value_size;

    Handle(std::shared_ptr<char> _value, uint64_t size) : value(_value), value_size(size){
    }
    ~Handle() {
    }
};

class SkipList {
public:
    SkipList();
    ~SkipList();

    void print();
    Status dump();
    Status load();
    Handle* search(const std::string& key) const;
    Status insert(const std::string& key, const char* value, uint64_t value_size);
    Status remove(const std::string& key);

    SkipList (const SkipList &sl) = delete;
    SkipList& operator=(const SkipList &sl) = delete;
    SkipList (const SkipList &&sl) = delete;
    SkipList& operator=(const SkipList &&sl) = delete;
private:
    int random_level();

    Node* list_head;
    Node* list_tail;
    uint64_t count;
    mutable std::shared_mutex mutex;

    static constexpr int MAX_LEVEL = ceil(log(UINT64_MAX) / log(1 / 0.25));
    static constexpr float _P = 0.25;
};

#endif //LDB_SKIPLIST_H
