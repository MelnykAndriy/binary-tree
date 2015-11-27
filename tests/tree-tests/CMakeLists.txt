include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(run_tree_tests tree-test.cpp performance-test.cpp)

target_link_libraries(run_tree_tests gtest gtest_main)
