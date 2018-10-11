//
// Created by happen on 9/26/18.
//

#include <glog/logging.h>

#include "node.h"

Node::Node(const std::string &key, const char *value, uint64_t value_size, int level) {
    _key = key;
    _value_size = value_size;
    char* new_value = new (std::nothrow)char[value_size];
    if (new_value != nullptr) {
        _value = std::shared_ptr<char>(new_value, \
                [](char* value_ptr) { SAFE_DELETE_ARR(value_ptr); });
        memcpy(new_value, value, value_size);
        _forward = std::vector<Node *>(level);
    } else {
        LOG(FATAL) << "Construct node faild, failed to allocate memory";
    }
}

Node::~Node() {
    _value.reset();
}