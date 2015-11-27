//
// Created by mandriy on 11/22/15.
//

#include "gtest/gtest.h"
#include "Tree.h"

#include <random>
#include <chrono>


using namespace std;

class BinaryTreePerformanceTest : public ::testing::Test {
public:

    virtual void SetUp() {

        default_random_engine generator((unsigned long)chrono::system_clock::now().time_since_epoch().count());
        uniform_real_distribution<double> distribution(-1000000, 1000000);

        for (int i=0; i < 1500000; i++) {
            double number = distribution(generator);
            hugeTree.insert(number);
            if ( i % 10000 == 0) {
                some_numbers.insert(number);
            }
        }
    }

    virtual void TearDown() {

    }

    Tree<double> hugeTree;
    set<double> some_numbers;
};

TEST_F(BinaryTreePerformanceTest, CountElements) {
    for (auto &number : some_numbers) {
        ASSERT_LE(1, hugeTree.countElements(number));
    }
}

TEST_F(BinaryTreePerformanceTest, ElementsRemoval) {
    for (auto &number : some_numbers) {
        ASSERT_LE(1, hugeTree.remove(number));
    }
}

TEST_F(BinaryTreePerformanceTest, ElementsRemoval2) {
    ASSERT_LT(0, hugeTree.removeAll([](const double& el) {
        return ((int) el)  % 2 == 1;
    }));
}

TEST_F(BinaryTreePerformanceTest, isMember) {
    for (auto &number : some_numbers) {
        ASSERT_TRUE(hugeTree.isMember(number));
    }
}

TEST_F(BinaryTreePerformanceTest, traverseTest) {
    for (int i = 0; i < 100; i++) {
        int traversed = 0;
        hugeTree.inOrderTraverse([&](const double& j) {
            traversed++;
        });
        ASSERT_EQ(traversed, hugeTree.size());
    }
}

TEST_F(BinaryTreePerformanceTest, makeSubtree) {
    ASSERT_LT(0, hugeTree.makeElementsSubtree([&](const double& el) {
        return ((int) el) % 2 == 0;
    }).size());
}

TEST_F(BinaryTreePerformanceTest, getSubtreeEl) {
    for(auto& number : some_numbers) {
        ASSERT_LE(1, hugeTree.getSubtreeFromElement(number).size());
    }
}
