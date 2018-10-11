//
// Created by happen on 9/15/18.
//

#include <fstream>
#include <iostream>
#include <random>

#include <glog/logging.h>

#include "skiplist.h"

SkipList::SkipList() : _count(0), _list_head(new Node("", nullptr, 0, MAX_LEVEL)), \
        _list_tail(new Node("", nullptr, 0, MAX_LEVEL)) {
    srand(time(0));
    for (int i = 0; i < MAX_LEVEL; ++i) {
        _list_head->_forward[i] = _list_tail;
    }
}


SkipList::~SkipList() {
    auto cur_node = _list_head->_forward[0];
    while (cur_node->_forward[0] != _list_tail) {
        auto next_node = cur_node->_forward[0];
        SAFE_DELETE(cur_node);
        cur_node = next_node;
    }
    SAFE_DELETE(_list_head);
    SAFE_DELETE(_list_tail);
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

Handle* SkipList::search(const std::string& key) const {
    std::shared_lock<std::shared_mutex> lock(_mutex);
    auto cur_node = _list_head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->_forward[i]->_key != "" && cur_node->_forward[i]->_key < key) {
            cur_node = cur_node->_forward[i];
        }
    }

    cur_node = cur_node->_forward[0];
    if (cur_node->_key == key) {
        auto handle = new(std::nothrow) Handle(cur_node->_value, cur_node->_value_size);
        if (handle == nullptr) {
            LOG(ERROR) << "Alloc handle failed";
        }
        return handle;
    }
    return nullptr;
}

Status SkipList::remove(const std::string& key) {
    std::unique_lock<std::shared_mutex> lock(_mutex);
    std::vector<Node*> update_forward_nodes(MAX_LEVEL);
    auto cur_node = _list_head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->_forward[i]->_key != "" && cur_node->_forward[i]->_key < key) {
            cur_node = cur_node->_forward[i];
        }
        update_forward_nodes[i] = cur_node;
    }

    cur_node = cur_node->_forward[0];

    if (cur_node->_key == key) {
        int level = cur_node->_forward.size();
        for (int i = 0; i < level; i++) {
            update_forward_nodes[i]->_forward[i] = cur_node->_forward[i];
        }
        SAFE_DELETE(cur_node);
        LOG(INFO) << "Remove " + key + " success";
        _count--;
        return Status::SUCCESS;
    }

    LOG(WARNING) << "Reomve failed, do not find " + key;
    return Status::FAIL;
}

Status SkipList::insert(const std::string& key, const char *value, uint64_t value_size) {
    std::unique_lock<std::shared_mutex> lock(_mutex);
    std::vector<Node*> update_forward_nodes(MAX_LEVEL);
    auto cur_node = _list_head;

    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        while (cur_node->_forward[i]->_key != "" && cur_node->_forward[i]->_key < key) {
            cur_node = cur_node->_forward[i];
        }
        update_forward_nodes[i] = cur_node;
    }

    cur_node = cur_node->_forward[0];

    if (cur_node->_key == key) {
        char* new_value = new (std::nothrow)char[value_size];
        if (new_value == nullptr) {
            LOG(FATAL) << "Construct node faild, failed to allocate memory";
            return Status::FAIL;
        }
        memcpy(new_value, value, value_size);
        cur_node->_value.reset(new_value);
        cur_node->_value_size = value_size;
        LOG(INFO) << "Update " + key + " success";
    } else {
        _count++;
        int level = random_level();
        Node* new_node = new Node(key, value, value_size, level);
        for (int i = 0; i < level; i++) {
            new_node->_forward[i] = update_forward_nodes[i]->_forward[i];
            update_forward_nodes[i]->_forward[i] = new_node;
        }
        LOG(INFO) << "Insert " + key + " success";
    }

    return Status::SUCCESS;
}

void SkipList::print() {
    std::shared_lock<std::shared_mutex> lock(_mutex);
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        auto cur_node = _list_head->_forward[i];
        while (cur_node != _list_tail) {
            std::cout << cur_node->_key + "->";
            cur_node = cur_node->_forward[i];
        }
        std::cout << "NIL" << std::endl;
    }
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        auto cur_node = _list_head->_forward[i];
        while (cur_node != _list_tail) {
            std::cout << std::string(cur_node->_value.get()) + "->";
            cur_node = cur_node->_forward[i];
        }
        std::cout << "NIL" << std::endl;
    }
}

Status SkipList::dump() {
    LOG(INFO) << "Dump skiplist to disk.";
    std::shared_lock<std::shared_mutex> lock(_mutex);
    std::fstream file("./skiplist.dump", std::ios::binary|std::ios::out);
    file.write(reinterpret_cast<char *>(&_count), sizeof(uint64_t));

    auto cur_node = _list_head->_forward[0];
    while (cur_node != _list_tail) {
        uint64_t key_size = cur_node->_key.size();
        auto key = cur_node->_key.c_str();
        file.write(reinterpret_cast<char *>(&key_size), sizeof(uint64_t));
        file.write(const_cast<char *>(key), key_size);
        file.write(reinterpret_cast<char *>(&cur_node->_value_size), sizeof(uint64_t));
        file.write(reinterpret_cast<char *>(cur_node->_value.get()), cur_node->_value_size);
        cur_node = cur_node->_forward[0];
    }

    file.close();

    return Status::SUCCESS;
}

Status SkipList::load() {
    LOG(INFO) << "Load skiplist from disk.";
    std::fstream file("./skiplist.dump", std::ios::binary|std::ios::in);
    if (!file || file.eof()) {
        return Status::SUCCESS;
    }

    uint64_t* insert_count = new uint64_t;
    std::shared_ptr<uint64_t> insert_count_ptr(insert_count);
    file.read(reinterpret_cast<char *>(insert_count), sizeof(uint64_t));
    auto count = *insert_count;

    while (count-- != 0) {
        uint64_t* key_size = new uint64_t;
        std::shared_ptr<uint64_t> key_size_ptr(key_size);
        file.read(reinterpret_cast<char *>(key_size), sizeof(uint64_t));

        char* key_cstr = new char[*key_size];
        std::shared_ptr<char> key_ptr(key_cstr);
        file.read(reinterpret_cast<char *>(key_cstr), *key_size);
        std::string key(key_cstr, *key_size);

        uint64_t* value_size = new uint64_t;
        std::shared_ptr<uint64_t> value_size_ptr(value_size);
        file.read(reinterpret_cast<char *>(value_size), sizeof(uint64_t));

        char* value = new char[*value_size];
        std::shared_ptr<char> value_ptr(value);
        file.read(value, *value_size);

        insert(key, value, *value_size);
    }

    return Status::SUCCESS;
}


