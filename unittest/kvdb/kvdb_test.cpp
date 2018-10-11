//
// Created by happen on 9/29/18.
//
#include <gtest/gtest.h>

#include "kvdb.h"

class TestKVDB:public testing::Test {
public:
    virtual void SetUp() {
        std::cout <<"Set Up Test" << std::endl;
        kvdb = KVDB::get_instance();
    }
    virtual void TearDown() {
        std::cout <<"Tear Down Test" << std::endl;
        kvdb->save();
    }

    KVDB* kvdb;
};

TEST_F(TestKVDB, setstr) {
    ASSERT_EQ(kvdb->set("1", "happen"), Status::SUCCESS);
    ASSERT_EQ(kvdb->set("2", "lee"), Status::SUCCESS);
    ASSERT_EQ(kvdb->set("happen", "1"), Status::SUCCESS);
    ASSERT_EQ(kvdb->set("lee", "2"), Status::SUCCESS);
}

TEST_F(TestKVDB, setvalue) {
    uint64_t x = 20;
    uint32_t y = 5;

    ASSERT_EQ(kvdb->set("20", reinterpret_cast<char *>(&x), 8), Status::SUCCESS);
    ASSERT_EQ(kvdb->set("5", reinterpret_cast<char *>(&y), 4), Status::SUCCESS);
}

TEST_F(TestKVDB, getstr) {
    ASSERT_STREQ(kvdb->get("1").c_str(), "happen");
    ASSERT_STREQ(kvdb->get("2").c_str(), "lee");
    ASSERT_STREQ(kvdb->get("happen").c_str(), "1");
    ASSERT_STREQ(kvdb->get("lee").c_str(), "2");
}

TEST_F(TestKVDB, getvalue) {
    uint64_t* x = new uint64_t;
    uint32_t* x_len = new uint32_t;
    uint32_t* y = new uint32_t;
    uint32_t* y_len = new uint32_t;

    ASSERT_EQ(kvdb->get("20", reinterpret_cast<char **>(&x), x_len), Status::SUCCESS);
    ASSERT_EQ(kvdb->get("5", reinterpret_cast<char **>(&y), y_len), Status::SUCCESS);

    ASSERT_EQ(*x, 20);
    ASSERT_EQ(*y, 5);
}

TEST_F(TestKVDB, remove) {
    ASSERT_EQ(kvdb->remove("1"), Status::SUCCESS);
    ASSERT_EQ(kvdb->get("1"), "");
    ASSERT_EQ(kvdb->remove("2"), Status::SUCCESS);
    ASSERT_EQ(kvdb->get("2"), "");


    ASSERT_EQ(kvdb->remove("20"), Status::SUCCESS);
    ASSERT_EQ(kvdb->remove("5"), Status::SUCCESS);

    uint64_t* x = new uint64_t;
    uint32_t* x_len = new uint32_t;
    uint32_t* y = new uint32_t;
    uint32_t* y_len = new uint32_t;
    ASSERT_EQ(kvdb->get("20", reinterpret_cast<char **>(&x), x_len), Status::FAIL);
    ASSERT_EQ(kvdb->get("5", reinterpret_cast<char **>(&y), y_len), Status::FAIL);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

