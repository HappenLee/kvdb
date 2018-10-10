#include <string>
#include <iostream>

#include "glog/logging.h"
#include "skiplist.h"

using namespace std;

int main(int argc, char* argv[]) {
    SkipList* sl = new SkipList();
    std::string test_string("happen");
    sl->insert("1", test_string.c_str(), test_string.size());
    test_string = "lee";
    sl->insert("2", test_string.c_str(), test_string.size());

    uint64_t data64 = 50;
    sl->insert("50", reinterpret_cast<char *>(&data64), 8);
    uint32_t data32 = 20;
    sl->insert("20", reinterpret_cast<char *>(&data32), 4);
    sl->search("1")->value_size == 6;
    std::string(sl->search("1")->value.get()) == "happen";
    sl->search("3") == nullptr;

    sl->remove("1") == Success;
    sl->remove("1") == Fail;
    sl->search("1") == nullptr;
    sl->dump();
    sl->load();

}