//
// Created by happen on 9/15/18.
//
#include "skiplist.h"
#include "kvdb.h"

KVDB* KVDB::singleton_kvdb = new KVDB();
KVDB::~KVDB() {
    skip_list.dump();
}

KVDB::KVDB() {
    skip_list.load();
}

void KVDB::save() {
    skip_list.dump();
}


void KVDB::get(const std::string key, char **value, uint32_t *value_size) {
    auto handle = skip_list.search(key);
    *value = reinterpret_cast<char*>(malloc(sizeof(char) * handle->value_size));
    memcpy(*value, handle->value.get(), handle->value_size);
    *value_size = handle->value_size;
}

std::string KVDB::get(const std::string key) {
    auto handle = skip_list.search(key);
    return std::string(handle->value.get());
}

void KVDB::set(const std::string key, const std::string value) {
    set(key, const_cast<char *>(value.c_str()), value.size());
}

void KVDB::set(const std::string key, char *value, uint32_t value_size) {
    skip_list.insert(key, value, value_size);
}

void KVDB::remove(const std::string key) {
    skip_list.remove(key);
}


