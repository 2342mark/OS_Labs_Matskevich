#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>

struct employee {
    int num;
    char name[10];
    double hours;
};


void printBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    std::cout << "\nContents of " << filename << ":\n";
    std::cout << "Num | Name     | Hours\n";
    std::cout << "----|----------|-------\n";

    employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::cout << emp.num << " | " << emp.name << " | " << static_cast<int>(emp.hours) << "\n";
    }

    file.close();
}

TEST(MainTest, PrintBinaryFile) {
    std::string filename = "test.bin";

  
    {
        std::ofstream outFile(filename, std::ios::binary);
        employee emp = {1, "Alice", 40.0};
        outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
        outFile.close();
    }


    std::ostringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    printBinaryFile(filename);

    std::cout.rdbuf(oldCoutBuffer);

    std::string output = buffer.str();
    EXPECT_NE(output.find("1 | Alice | 40"), std::string::npos);  
}