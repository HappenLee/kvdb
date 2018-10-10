//
// Created by happen on 9/29/18.
//


#include <gtest/gtest.h>

#include "skiplist.h"

class Test_Skiplist:public testing::Test {
public:
    virtual void SetUp() {
        std::cout <<"Set Up Test" << std::endl;
        sl->load();

    }
    virtual void TearDown() {
        std::cout <<"Tear Down Test" << std::endl;
        sl->dump();
    }

    SkipList* sl = new SkipList();
};

TEST_F(Test_Skiplist, insert) {
    std::string test_string("happen");
    ASSERT_EQ(sl->insert("1", test_string.c_str(), test_string.size()), Success);
    test_string = "lee";
    ASSERT_EQ(sl->insert("2", test_string.c_str(), test_string.size()), Success);

    uint64_t data64 = 50;
    ASSERT_EQ(sl->insert("50", reinterpret_cast<char *>(&data64), 8), Success);
    uint32_t data32 = 20;
    ASSERT_EQ(sl->insert("20", reinterpret_cast<char *>(&data32), 4), Success);
}

TEST_F(Test_Skiplist, search) {
    ASSERT_EQ(sl->search("1")->value_size, 6);
    ASSERT_STREQ(std::string(sl->search("1")->value.get()).c_str(), "happen");

    ASSERT_EQ(sl->search("3"), nullptr);
}

TEST_F(Test_Skiplist, remove) {
    ASSERT_EQ(sl->remove("1"), Success);
    ASSERT_EQ(sl->remove("1"), Fail);

    ASSERT_EQ(sl->search("1"), nullptr);
}

TEST_F(Test_Skiplist, dump) {
    ASSERT_EQ(sl->dump(), Success);
}

TEST_F(Test_Skiplist, load) {
    ASSERT_EQ(sl->load(), Success);
}



int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}