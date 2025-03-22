#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: Reporter <binary_file> <report_file> <pay_per_hour>\n";
        return 1;
    }

    std::string binaryFilename = argv[1];
    std::string reportFilename = argv[2];
    double payPerHour = std::stod(argv[3]);

    std::ifstream inFile(binaryFilename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Cannot open file: " << binaryFilename << "\n";
        return 1;
    }

    std::ofstream outFile(reportFilename);
    if (!outFile) {
        std::cerr << "Cannot create report file: " << reportFilename << "\n";
        return 1;
    }

   
    outFile << "Report for file \"" << binaryFilename << "\"\n";
    outFile << "Employee number | Employee name | Hours | Salary\n";

    employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        double salary = emp.hours * payPerHour;
        outFile << std::setw(15) << emp.num << " | "
                << std::setw(13) << emp.name << " | "
                << std::setw(5) << emp.hours << " | "
                << std::setw(6) << salary << "\n";
    }

    inFile.close();
    outFile.close();
    return 0;
}