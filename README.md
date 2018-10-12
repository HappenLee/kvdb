[中文版](README_cn.md)

### 1. The Design KVDB

The K-V storage engine is implemented by implementing a data structure of [Skip list] (https://en.wikipedia.org/wiki/Skip_list). **SkipList** is a simple and efficient data structure for querying. It can achieve query time complexity (logn) similar to **AVL tree and Red-Black tree**。 Many popular databases use SkipList to encapsulate storage engines, such as **LevelDB, Cassandra, Rocksdb**.


The implementation of the jump table is very simple. It is a multi level linked list structure below:

![Skip table structure](https://upload-images.jianshu.io/upload_images/8552201-35bd4ca1104c15c1.png?ImageMogr2/auto-orient/strip%7CimageView2/2/w/1240)


In the concrete realization, the author refers to the conclusion of the paper, sets the probability of node appearing in each level to 0.25, and sets the level to 32. Also, for simplicity, use the std:: string structure as the key in the jump table, and save the serialized value through char* and value_size. At the same time, use the read and write lock in C++17: **std:: share_mutex** to ensure thread safety of multithreading and writing.


The author uses **glog** as a logging system, initializes glog to log when kvdb starts, and uses **gtest** to unit test.

### 2. Performance Test

The virtual machine of my own notebook carried out a simple performance test. Since the author is developing through the personal MAC virtual machine, so the performance test is the result of the virtual machine run out, there may be some deviation:

#### Configuration information:

CPU: Intel i5-7360u dual core

MEM: 4GB

Operating System: Deepin 15.15

#### Test results:

Here I use the STL data structure: **map, unordered_map, and the author's implementation of the kvdb performance test**, mainly testing the insertion of data and query of data, the results are really desperate.

|  Data   | kvdb | map | unordered _map |
|  ------  | ------ | ------ | ------ |
|  10W   | 1.02s | 0.33s | 0.18s |
|  50W | 5.49s | 1.69s | 1.07s |
|  100W | 14.03s | 3.68s | 2.20s |


### 3. Instructions For Use

**KVDB** is very simple to use, and the whole project is built through cmake. After compiling the static library file of libkvdb.a, you can call the API to use it.

The list of APIs is as follows. KVDB is a singleton object that can be used as a standalone storage engine after obtaining the object pointer. And save can be used to achieve persistent storage of data. The following is the interface provided by KVDB:
```
static KVDB* get_instance() {
        return singleton_kvdb;
    }
    void get(const std::string key, char** value, uint32_t *value_size);
    std::string get(const std::string key);
    void set(const std::string key, char* value, uint32_t value_size);
    void set(const std::string key, const std::string value);
    void remove(const std::string);

    void load();
    void save();
```



