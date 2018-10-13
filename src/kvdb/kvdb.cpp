//
// Created by happen on 9/15/18.
//
#include "glog/logging.h"

#include "kvdb.h"
#include "skiplist.h"

KVDB* KVDB::_s_singleton_kvdb = new KVDB();
KVDB::~KVDB() {
}

KVDB::KVDB() {
    google::InitGoogleLogging("KVDB");
    _skip_list.load();
}

void KVDB::save() {
    _skip_list.dump();
}

Status KVDB::get(const std::string &key, char **value, uint32_t *value_size) {
    auto handle = _skip_list.search(key);
    if (handle != nullptr) {
        *value = reinterpret_cast<char *>(malloc(sizeof(char) * handle->value_size));
        memcpy(*value, handle->value.get(), handle->value_size);
        *value_size = handle->value_size;
        return Status::SUCCESS;
    }

    return Status::FAIL;
}

std::string KVDB::get(const std::string &key) {
    auto handle = _skip_list.search(key);
    if (nullptr != handle) {
        return std::string(handle->value.get(), handle->value_size);
    }
    LOG(WARNING) << "Don't fine the " + key;
    return "";
}

Status KVDB::set(const std::string &key, const std::string value) {
    return set(key, const_cast<char *>(value.c_str()), value.size());
}

Status KVDB::set(const std::string &key, char *value, uint32_t value_size) {
    return _skip_list.insert(key, value, value_size);
}

Status KVDB::remove(const std::string &key) {
    return _skip_list.remove(key);
}


