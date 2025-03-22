#include <iostream>
#include <vector>
#include <windows.h>

using namespace std;

struct ThreadData {
    vector<int> numbers;
    int min;
    int max;
    double average;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    if (data->numbers.empty()) return 1;

    data->min = data->max = data->numbers[0];

    for (int num : data->numbers) {
        if (num < data->min) data->min = num;
        if (num > data->max) data->max = num;
        Sleep(7);  
    }

    cout << "Min: " << data->min << ", Max: " << data->max << endl;
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    ThreadData* data = static_cast<ThreadData*>(lpParam);
    if (data->numbers.empty()) return 1;

    long long sum = 0;
    for (int num : data->numbers) {
        sum += num;
        Sleep(12);
    }

    data->average = static_cast<double>(sum) / data->numbers.size();
    cout << "Average: " << data->average << endl;
    return 0;
}

int main() {
    int size;
    cout << "Enter array size: ";
    cin >> size;

    ThreadData data;
    data.numbers.resize(size);

    cout << "Enter " << size << " integers:" << endl;
    for (int i = 0; i < size; ++i) {
        cin >> data.numbers[i];
    }

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &data, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &data, 0, NULL);

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    for (int& num : data.numbers) {
        if (num == data.min || num == data.max) {
            num = static_cast<int>(data.average);
        }
    }

    cout << "Modified array: ";
    for (int num : data.numbers) {
        cout << num << " ";
    }
    cout << endl;
    
    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    return 0;
}
