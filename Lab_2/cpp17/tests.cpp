#include <gtest/gtest.h>
#define main main_  
#include "main.cpp" 
#undef main 

TEST(ThreadTests, MinMaxTest) {
    ThreadData data;
    data.numbers = {5, 10, 3, 8, 12};
    
    MinMaxThread(&data);
    
    EXPECT_EQ(data.min, 3);
    EXPECT_EQ(data.max, 12);
}

TEST(ThreadTests, AverageTest) {
    ThreadData data;
    data.numbers = {5, 10, 3, 8, 14};
    
    AverageThread(&data);
    
    EXPECT_DOUBLE_EQ(data.average, 8.0);
}

TEST(ThreadTests, ReplaceMinMaxLogic) {
    ThreadData data;
    data.numbers = {3, 10, 8, 12, 3};
    data.min = 3;
    data.max = 12;
    data.average = 9.0;
    
    
    for (int& num : data.numbers) {
        if (num == data.min || num == data.max) {
            num = static_cast<int>(data.average);
        }
    }
    
    std::vector<int> expected = {9, 10, 8, 9, 9};
    EXPECT_EQ(data.numbers, expected);
}
