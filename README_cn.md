### 1.KVDB的设计思路
通过实现了一个[Skiplist](https://en.wikipedia.org/wiki/Skip_list)的数据结构来实现的K-V存储引擎。**SkipList**是一种实现简单，查询高效的数据结构。能够实现与**AVL树，红黑树**相近的查询时间复杂度（logn）。许多流行的数据库都利用SkipList封装了存储引擎，如：**LevelDB，Cassandra，Rocksdb**等。

跳表的实现十分简单，它是一个下面的多级链表结构：
![跳表的结构](https://upload-images.jianshu.io/upload_images/8552201-35bd4ca1104c15c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在具体的实现之中，笔者参考了论文之中的结论，将每个层级出现了node的概率设置为了0.25，将层级设为了32级。同时为了简单的起见，直接通过std::string结构作为跳表之中的Key，同时通过char\*与value_size来保存序列化之后的value。同时利用C++17之中的读写锁：**std::share_mutex**来确保多线程读写的线程安全。

笔者这里使用了glog作为日志记录系统，在kvdb启动时便初始化glog进行日志记录，同时利用gtest进行单元测试。

### 2.性能测试
笔者自己的笔记本上的虚拟机进行了简单的性能测试 。由于笔者是通过个人mac的虚拟机进行开发，所以性能测试是虚拟机跑出来的结果，可能有所偏差：
#### 配置信息：
CPU：Intel i5-7360u 双核

MEM：4GB

Operating System: Deepin 15. 15
#### 测试结果：
这里笔者使用了STL之中的数据结构： **map，unordered_map，与笔者实现的kvdb进行对比性能测试**，主要测试了数据的插入与查询，结果真是让人绝望，哈哈哈~~

|  数据   | kvdb | map | unordered _map |
|  ------  | ------ | ------ | ------ |
|  10W   | 1.02s | 0.33s | 0.18s |
|  50W | 5.49s | 1.69s | 1.07s |
|  100W | 14.03s | 3.68s | 2.20s |

### 3.使用说明
**KVDB**使用起来非常简单，整个项目通过cmake来构建项目。通过编译生成libkvdb.a的静态库文件之后，就可以调用其API进行使用了。
API列表如下，KVDB是一个单例对象，获取对象指针之后就可以作为单机存储引擎来使用。并且可以通过save方法来实现数据的持久化存储。下面是KVDB提供的使用接口：
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
