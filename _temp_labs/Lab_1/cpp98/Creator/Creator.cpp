#include <iostream>
#include <fstream>
#include <string>

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: Creator <binary_file> <record_count>\n";
        return 1;
    }

    std::string filename = argv[1];
    int recordCount = atoi(argv[2]); 

    std::ofstream outFile(filename.c_str(), std::ios::binary);
    if (!outFile) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return 1;
    }

    for (int i = 0; i < recordCount; ++i) {
        employee emp;
        std::cout << "Enter employee " << i + 1 << " (num name hours): ";
        std::cin >> emp.num >> emp.name >> emp.hours;
        outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    outFile.close();
    return 0;
}