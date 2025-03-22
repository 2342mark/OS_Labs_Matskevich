#include <gtest/gtest.h>
#include <fstream>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

TEST(CreatorTest, WriteAndReadEmployee) {
    std::string filename = "test.bin";
    std::ofstream outFile(filename, std::ios::binary);
    employee emp = {1, "Alice", 40.0};
    outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    outFile.close();

    std::ifstream inFile(filename, std::ios::binary);
    employee readEmp;
    inFile.read(reinterpret_cast<char*>(&readEmp), sizeof(employee));

    EXPECT_EQ(emp.num, readEmp.num);
    EXPECT_STREQ(emp.name, readEmp.name);
    EXPECT_EQ(emp.hours, readEmp.hours);
}