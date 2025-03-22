#include <iostream>
#include <fstream>
#include <string>
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
        std::cout << std::setw(3) << emp.num << " | "
                  << std::setw(8) << emp.name << " | "
                  << emp.hours << "\n";
    }

    file.close();
}

void printReportFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << "\n";
        return;
    }

    std::cout << "\nContents of report " << filename << ":\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    file.close();
}

int main() {
   
    std::string binaryFilename;
    int recordCount;
    
    std::cout << "Enter binary file name: ";
    std::cin >> binaryFilename;
    std::cout << "Enter number of records: ";
    std::cin >> recordCount;


    std::string creatorCmd = "Creator " + binaryFilename + " " + std::to_string(recordCount);
    
    
    STARTUPINFOW siCreator;
    PROCESS_INFORMATION piCreator;
    ZeroMemory(&siCreator, sizeof(siCreator));
    siCreator.cb = sizeof(siCreator);
    ZeroMemory(&piCreator, sizeof(piCreator));

 
    std::wstring wCreatorCmd(creatorCmd.begin(), creatorCmd.end());

    if (!CreateProcessW(
        NULL,
        const_cast<LPWSTR>(wCreatorCmd.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &siCreator,
        &piCreator
    )) {
        std::cerr << "CreateProcess failed for Creator (" << GetLastError() << ")\n";
        return 1;
    }

  
    WaitForSingleObject(piCreator.hProcess, INFINITE);
    CloseHandle(piCreator.hProcess);
    CloseHandle(piCreator.hThread);

 
    printBinaryFile(binaryFilename);

  
    std::string reportFilename;
    double payPerHour;
    
    std::cout << "\nEnter report file name: ";
    std::cin >> reportFilename;
    std::cout << "Enter pay per hour: ";
    std::cin >> payPerHour;

    // Step 6: Run Reporter
    std::string reporterCmd = "Reporter.exe " + binaryFilename + " " + reportFilename + " " + std::to_string(payPerHour);
    
    STARTUPINFOW siReporter;
    PROCESS_INFORMATION piReporter;
    ZeroMemory(&siReporter, sizeof(siReporter));
    siReporter.cb = sizeof(siReporter);
    ZeroMemory(&piReporter, sizeof(piReporter));


    std::wstring wReporterCmd(reporterCmd.begin(), reporterCmd.end());

    if (!CreateProcessW(
        NULL,
        const_cast<LPWSTR>(wReporterCmd.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &siReporter,
        &piReporter
    )) {
        std::cerr << "CreateProcess failed for Reporter (" << GetLastError() << ")\n";
        return 1;
    }

  
    WaitForSingleObject(piReporter.hProcess, INFINITE);
    CloseHandle(piReporter.hProcess);
    CloseHandle(piReporter.hThread);

 
    printReportFile(reportFilename);

    return 0;
}