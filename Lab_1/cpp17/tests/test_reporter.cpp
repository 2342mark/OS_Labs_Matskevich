#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#pragma pack(push, 1)
struct employee { 
    int num; 
    char name[10]; 
    int hours; 
};
#pragma pack(pop)

TEST(ReporterTest, GenerateReport) {
    std::string binaryFilename = "test.bin";
    std::string reportFilename = "report.txt";
    int payPerHour = 15;

    
    employee emp = {1, "", 40};
    strncpy(emp.name, "Alice", sizeof(emp.name) - 1); 

    std::ofstream outFile(binaryFilename, std::ios::binary);
    ASSERT_TRUE(outFile.is_open());
    outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    outFile.close();


    std::ifstream inFile(binaryFilename, std::ios::binary);
    ASSERT_TRUE(inFile.is_open());
    std::ofstream reportFile(reportFilename);
    ASSERT_TRUE(reportFile.is_open());

    reportFile << "Report for file \"" << binaryFilename << "\"\n";
    reportFile << "Employee number|Employee name|Hours|Salary\n";

    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        std::string name(emp.name, strnlen(emp.name, sizeof(emp.name)));
        int salary = emp.hours * payPerHour;
        reportFile << emp.num << "|" << name << "|" << emp.hours << "|" << salary << "\n";
    }

    inFile.close();
    reportFile.close();

    std::ifstream reportInput(reportFilename);
    ASSERT_TRUE(reportInput.is_open());

    std::string line;

    std::getline(reportInput, line); 
    std::getline(reportInput, line); 

    std::getline(reportInput, line); 

  
    line.erase(std::remove_if(line.begin(), line.end(), 
        [](char c) { return c == '\r' || c == '\n' || c == ' '; }), line.end());

    EXPECT_EQ(line, "1|Alice|40|600");
}
