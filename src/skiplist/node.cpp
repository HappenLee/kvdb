//
// Created by happen on 9/26/18.
//

#include <glog/logging.h>
#include "node.h"

Node::Node(const std::string &key, const char *value, uint64_t value_size, int level) {
    this->key = key;
    this->value_size = value_size;
    char* new_value = new (std::nothrow)char[value_size];
    if (new_value != nullptr) {
        this->value = std::shared_ptr<char>(new_value, [](char* value_ptr) { SAFE_DELETE_ARR(value_ptr); });
        memcpy(new_value, value, value_size);
        forward = std::vector<Node *>(level);
    } else {
        LOG(FATAL) << "Construct node faild, failed to allocate memory";
    }
}

Node::~Node() {
    this->value.reset();
}