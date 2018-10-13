//
// Created by happen on 9/15/18.
//

#pragma once
#ifndef LDB_KVDB_H
#define LDB_KVDB_H

#include "skiplist.h"

class KVDB {
public:
    static KVDB* get_instance() {
        return _s_singleton_kvdb;
    }
    Status get(const std::string &key, char** value, uint32_t *value_size);
    std::string get(const std::string &key);
    Status set(const std::string &key, char* value, uint32_t value_size);
    Status set(const std::string &key, const std::string value);
    Status remove(const std::string &key);

    void load();
    void save();

    KVDB (const KVDB &db) = delete;
    KVDB& operator=(const KVDB &db) = delete;
    KVDB (const KVDB &&db) = delete;
    KVDB& operator=(const KVDB &&db) = delete;
private:
    static KVDB* _s_singleton_kvdb;
    KVDB();
    ~KVDB();
    SkipList _skip_list;
};


#endif //LDB_KVDB_H
