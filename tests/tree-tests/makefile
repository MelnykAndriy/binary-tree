
GTEST_PATH ?= ../lib/gtest-1.7.0
EXECUTABLE_NAME ?= tree-performance
OUT_FILE ?= tree-performance.out
TREE_TESTS += performance-test.cpp
INCLUDE += ../../containers $(GTEST_PATH)/include
GTEST = $(foreach file,gtest-all.o gtest_main.o,$(GTEST_PATH)/make/$(file))
FLAGS += -std=c++11 -pthread

.PHONY : run-profiling
.PHONY : clean

all : run-profiling

run-profiling : build-profiling
	echo "Running profiling ..."
	valgrind --tool=callgrind --callgrind-out-file=$(OUT_FILE) ./$(EXECUTABLE_NAME)
	echo "Done!"

build-profiling : $(TREE_TESTS) $(GTEST)
	g++ $(TREE_TESTS) $(GTEST) $(foreach dir,$(INCLUDE), -I $(dir)) $(FLAGS) -o $(EXECUTABLE_NAME)

$(GTEST) :
	$(MAKE) -C $(GTEST_PATH)/make

clean : 
	$(MAKE) -C $(GTEST_PATH)/make clean
	rm -rf $(EXECUTABLE_NAME) $(OUT_FILE)