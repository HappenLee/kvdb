//
// Created by happen on 9/15/18.
//
#include <random>
#include <iostream>
#include <fstream>

#include "skiplist.h"
#include "common.h"

SkipList::SkipList():count(0) {
    srand(time(0));
    list_head = new Node("", nullptr, 0, MAX_LEVEL);
    list_tail = new Node("", nullptr, 0, MAX_LEVEL);
    for (int i = 0; i < MAX_LEVEL; ++i) {
        list_head->forward[i] = list_tail;
    }
}


SkipList::~SkipList() {
    auto cur_node = list_head->forward[0];
    while (cur_node->forward[0] != list_tail) {
        auto next_node = cur_node->forward[0];
        SAFE_DELETE(cur_node);
        cur_node = next_node;
    }
    SAFE_DELETE(list_head);
    SAFE_DELETE(list_tail);
}

int SkipList::random_level() {
    int level = 1;

    while (level <= MAX_LEVEL) {
        auto num = (rand() % 100) / 101.0;
        if (num < _P) {
            level++;
        }
        else {
            break;
        }
    }
    return level;
}

Handle* SkipList::search(std::string key) const {
    std::shared_lock<std::shared_mutex> lock(mutex);
    auto cur_node = list_head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->forward[i]->key != "" && cur_node->forward[i]->key < key) {
                cur_node = cur_node->forward[i];
        }
    }

    cur_node = cur_node->forward[0];
    if (cur_node->key == key) {
        return new Handle(cur_node->value, cur_node->value_size);
    }
    return nullptr;
}

void SkipList::remove(std::string key) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    std::vector<Node*> update_forward_nodes(MAX_LEVEL);
    auto cur_node = list_head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->forward[i]->key != "" && cur_node->forward[i]->key < key) {
            cur_node = cur_node->forward[i];
        }
        update_forward_nodes[i] = cur_node;
    }

    cur_node = cur_node->forward[0];

    if (cur_node->key == key) {
        int level = cur_node->forward.size();
        for (int i = 0; i < level; i++) {
            update_forward_nodes[i]->forward[i] = cur_node->forward[i];
        }
        SAFE_DELETE(cur_node);
    }
}

void* SkipList::insert(std::string key, const char *value, uint64_t value_size) {
    std::unique_lock<std::shared_mutex> lock(mutex);
    std::vector<Node*> update_forward_nodes(MAX_LEVEL);
    auto cur_node = list_head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->forward[i]->key != "" && cur_node->forward[i]->key < key) {
            cur_node = cur_node->forward[i];
        }
        update_forward_nodes[i] = cur_node;
    }

    cur_node = cur_node->forward[0];

    if (cur_node->key == key) {
        char* new_value = new char[value_size];
        memcpy(new_value, value, value_size);
        cur_node->value.reset(new_value);
        cur_node->value_size = value_size;
    } else {
        count++;
        int level = random_level();
        Node* new_node = new Node(key, value, value_size, level);
        for (int i = 0; i < level; i++) {
            new_node->forward[i] = update_forward_nodes[i]->forward[i];
            update_forward_nodes[i]->forward[i] = new_node;
        }
    }
}

void SkipList::print() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        auto cur_node = list_head->forward[i];
        while (cur_node != list_tail) {
            std::cout << cur_node->key + "->";
            cur_node = cur_node->forward[i];
        }
        std::cout << "NIL" << std::endl;
    }
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        auto cur_node = list_head->forward[i];
        while (cur_node != list_tail) {
            std::cout << std::string(cur_node->value.get()) + "->";
            cur_node = cur_node->forward[i];
        }
        std::cout << "NIL" << std::endl;
    }
}

void SkipList::dump() {
    std::shared_lock<std::shared_mutex> lock(mutex);
    std::fstream file("/home/happen/skiplist.dump", std::ios::binary|std::ios::out);
    file.write(reinterpret_cast<char *>(&count), 8);

    auto cur_node = list_head->forward[0];
    while (cur_node != list_tail) {
        auto key_size = cur_node->key.size();
        auto key = cur_node->key.c_str();
        file.write(reinterpret_cast<char *>(&key_size), 8);
        file.write(const_cast<char *>(key), key_size);
        file.write(reinterpret_cast<char *>(&cur_node->value_size), 8);
        file.write(reinterpret_cast<char *>(cur_node->value.get()), cur_node->value_size);
        cur_node = cur_node->forward[0];
    }

    file.close();
}

void SkipList::load() {
    std::fstream file("/home/happen/skiplist.dump", std::ios::binary|std::ios::in);
    uint64_t* insert_count = new uint64_t;
    std::shared_ptr<uint64_t> insert_count_ptr(insert_count);
    file.read(reinterpret_cast<char *>(insert_count), 8);
    auto count = *insert_count;

    while (count-- != 0) {
        size_t* key_size = new size_t;
        std::shared_ptr<size_t> key_size_ptr(key_size);
        file.read(reinterpret_cast<char *>(key_size), 8);

        char* key_cstr = new char[*key_size];
        std::shared_ptr<char> key_ptr(key_cstr);
        file.read(reinterpret_cast<char *>(key_cstr), *key_size);
        std::string key(key_cstr, *key_size);

        uint64_t* value_size = new uint64_t;
        std::shared_ptr<uint64_t> value_size_ptr(value_size);
        file.read(reinterpret_cast<char *>(value_size), 8);

        char* value = new char[*value_size];
        std::shared_ptr<char> value_ptr(value);
        file.read(value, *value_size);

        insert(key, value, *value_size);
    }
}

