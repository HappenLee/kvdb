//
// Created by happen on 9/29/18.
//


#include <gtest/gtest.h>

#include "skiplist.h"

class Test_Skiplist : public testing::Test {
public:
    virtual void SetUp() {
        std::cout << "Set Up Test" << std::endl;
        _sl->load();

    }
    virtual void TearDown() {
        std::cout << "Tear Down Test" << std::endl;
        _sl->dump();
    }

    ~Test_Skiplist(){};

    SkipList* _sl = new SkipList();
};

TEST_F(Test_Skiplist, insert) {
    std::string test_string("happen");
    ASSERT_EQ(_sl->insert("1", test_string.c_str(), test_string.size()), Status::SUCCESS);
    test_string = "lee";
    ASSERT_EQ(_sl->insert("2", test_string.c_str(), test_string.size()), Status::SUCCESS);

    uint64_t data64 = 50;
    ASSERT_EQ(_sl->insert("50", reinterpret_cast<char *>(&data64), 8), Status::SUCCESS);
    uint32_t data32 = 20;
    ASSERT_EQ(_sl->insert("20", reinterpret_cast<char *>(&data32), 4), Status::SUCCESS);
}

TEST_F(Test_Skiplist, search) {
    ASSERT_EQ(_sl->search("1")->value_size, 6);
    ASSERT_STREQ(std::string(_sl->search("1")->value.get()).c_str(), "happen");

    ASSERT_EQ(_sl->search("3"), nullptr);
}

TEST_F(Test_Skiplist, remove) {
    ASSERT_EQ(_sl->remove("1"), Status::SUCCESS);
    ASSERT_EQ(_sl->remove("1"), Status::FAIL);

    ASSERT_EQ(_sl->search("1"), nullptr);
}

TEST_F(Test_Skiplist, dump) {
    ASSERT_EQ(_sl->dump(), Status::SUCCESS);
}

TEST_F(Test_Skiplist, load) {
    ASSERT_EQ(_sl->load(), Status::SUCCESS);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}
