//
// Created by mandriy on 11/16/15.
//

#include "gtest/gtest.h"
#include "Tree.h"

#include <string>
#include <vector>
#include <functional>

class BinaryTreeTest : public ::testing::Test {
public:

    virtual void SetUp() {
        startsFromA = [](const std::string& name)->bool {
            return name[0] == 'A';
        };


        /// Insert some data into name_tree
        names_not_from_a.push_back("Leha");
        names_not_from_a.push_back("Petia");
        names_not_from_a.push_back("Slava");
        names_not_from_a.push_back("Konstantin");
        names_not_from_a.push_back("Evgenija");
        names_not_from_a.push_back("Vika");


        for (int i = 0; i < 3; i++) {
            name_tree.insert("Andriy");
        }

        for (int i = 0; i < 5; i++) {
            name_tree.insert("Anton");
        }

        for (auto &name : names_not_from_a) {
            name_tree.insert(name);
        }

        name_tree.insert("Alex");
        name_tree.insert("Artem");

        // numbers tree
        for (double x = 20.0; x > 1.5; x -= 1.4) {
            decreasing_numbers_tree.insert(x);
        }
        for (double x = 1.5; x < 20.0; x += 1.2) {
            increasing_numbers_tree.insert(x);
        }
    }

    virtual void TearDown() {

    }


    Tree<int> initially_empty_tree;
    Tree<std::string> name_tree;
    Tree<double> decreasing_numbers_tree;
    Tree<double> increasing_numbers_tree;
    std::vector<std::string> names_not_from_a;

    std::function<bool(const std::string &)> startsFromA;
};

TEST_F(BinaryTreeTest, TreeWorks) {
    EXPECT_EQ(0, initially_empty_tree.size()) << "Tree should be initially empty";
    for (int i = 0; i < 10; i++) {
        initially_empty_tree.insert(i);
    }
    EXPECT_EQ(10, initially_empty_tree.size()) << "Tree is not empty, because we insert some data";
    EXPECT_TRUE(initially_empty_tree.isMember(5)) << "Element is inside of tree";
    EXPECT_FALSE(initially_empty_tree.isMember(-1)) << "Element is not inside of tree";
    initially_empty_tree.clear();
    EXPECT_EQ(0, initially_empty_tree.size()) << "Tree is empty again";
}


TEST_F(BinaryTreeTest, WorkWithElements) {
    EXPECT_EQ(3, name_tree.countElements("Andriy"));
    EXPECT_EQ(10, name_tree.countElements(startsFromA));
}

TEST_F(BinaryTreeTest, MakingSubtrees1) {
    auto new_tree = name_tree.makeElementsSubtree([&](const std::string& name) {
        return !startsFromA(name);
    });

    ASSERT_EQ(names_not_from_a.size(), new_tree.size());

    for (auto &name : names_not_from_a) {
        EXPECT_TRUE(new_tree.isMember(name));
    }

}

TEST_F(BinaryTreeTest, MakingSubtrees2) {
    auto new_tree = name_tree.getSubtreeFromElement("Slava");
    EXPECT_EQ(2, new_tree.size());
    EXPECT_TRUE(new_tree.isMember("Vika"));
    EXPECT_TRUE(new_tree.isMember("Slava"));
}

TEST_F(BinaryTreeTest, MakingSubtrees3) {
    auto new_tree = name_tree.getSubtreeFromElement([](const std::string& name)->bool {
        return name.length() == 5;
    });

    EXPECT_FALSE(new_tree.size() == 0);
}

TEST_F(BinaryTreeTest, ElementsRemoval1) {
    EXPECT_EQ(3, name_tree.countElements("Andriy"));
    EXPECT_EQ(3, name_tree.removeAll("Andriy")) << "Remove elements";
    ASSERT_EQ(0, name_tree.countElements("Andriy")) << "Check wheter all elements are removed";

    EXPECT_EQ(7, name_tree.countElements(startsFromA));
    EXPECT_EQ(7, name_tree.removeAll(startsFromA));
    ASSERT_EQ(0, name_tree.countElements("Andriy")) << "Check wheter all elements are removed";
}

TEST_F(BinaryTreeTest, ElementsRemoval2) {

    EXPECT_EQ(2, name_tree.remove("Andriy", 2));
    EXPECT_EQ(1, name_tree.countElements("Andriy"));

    EXPECT_EQ(1, name_tree.remove("Anton"));
    EXPECT_EQ(4, name_tree.countElements("Anton"));

    EXPECT_EQ(4, name_tree.remove("Anton", 5));
    EXPECT_EQ(0, name_tree.countElements("Anton"));
}

TEST_F(BinaryTreeTest, EmptyTreeTraverse) {
    int visited = 0;
    auto traverse_func = [&](const int& i) {
        visited++;
    };
    initially_empty_tree.inOrderTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "inOrderTraverse";
    visited = 0;
    initially_empty_tree.inOppositeOrderTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "inOppositeOrderTraverse";
    visited = 0;
    initially_empty_tree.preLeftTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "preLeftTraverse";
    visited = 0;
    initially_empty_tree.preRightTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "preRightTraverse";
    visited = 0;
    initially_empty_tree.postLeftTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "postLeftTraverse";
    visited = 0;
    initially_empty_tree.postRightTraverse(traverse_func);
    EXPECT_EQ(0, visited) << "postRightTraverse";
    visited = 0;
}

TEST_F(BinaryTreeTest, InOrderTraverse) {
    auto test_in_order = [] (Tree<double> &tree) {
        double prev = 0;

        tree.inOrderTraverse([&](const double &x) {
            EXPECT_GE(x, prev);
            prev = x;
        });
    };

    test_in_order(increasing_numbers_tree);
    test_in_order(decreasing_numbers_tree);
}

TEST_F(BinaryTreeTest, InOppositeTraverse) {
    auto test_in_opposite_order = [](Tree<double> &tree) {
        double prev = 100;

        tree.inOppositeOrderTraverse([&](const double &x) {
            EXPECT_LE(x, prev);
            prev = x;
        });
    };

    test_in_opposite_order(increasing_numbers_tree);
    test_in_opposite_order(decreasing_numbers_tree);
}

TEST_F(BinaryTreeTest, InOrderTraverseWithStopCondition) {
    auto test_order = [](Tree<double> &tree) {
        tree.inOrderTraverse(
                [&](const double& x){
                    EXPECT_GT(10, x);
                },
                [&](const double& x) -> bool {
                    return x > 10;
                });
    };

    test_order(increasing_numbers_tree);
    test_order(decreasing_numbers_tree);
}

TEST_F(BinaryTreeTest, InOpositeOrderTraverseWithStopCondition) {
    auto test_opposite_order = [](Tree<double> &tree, std::string msg) {
        tree.inOppositeOrderTraverse(
                [&](const double& x){
                    EXPECT_LT(10, x) << msg;
                },
                [&](const double& x) -> bool {
                    return x < 10;
                });
    };

    test_opposite_order(increasing_numbers_tree, "Increasing");
    test_opposite_order(decreasing_numbers_tree, "Decreasing");
}

TEST_F(BinaryTreeTest, TestOtherTraversals) {

#define TEST_TRAVERSAL(traversal) { \
    unsigned int visited = 0; \
    traversal([&](const double& el){ \
        visited++; \
    }); \
    EXPECT_EQ(increasing_numbers_tree.size(), visited); \
 };

    TEST_TRAVERSAL(increasing_numbers_tree.inOrderTraverse);
    TEST_TRAVERSAL(increasing_numbers_tree.inOppositeOrderTraverse);
    TEST_TRAVERSAL(increasing_numbers_tree.preLeftTraverse);
    TEST_TRAVERSAL(increasing_numbers_tree.preRightTraverse);
    TEST_TRAVERSAL(increasing_numbers_tree.postLeftTraverse);
    TEST_TRAVERSAL(increasing_numbers_tree.postRightTraverse);
}



// tree traversals
