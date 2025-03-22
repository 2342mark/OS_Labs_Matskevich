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
    ThreadData* data = (ThreadData*)lpParam;
    if (data->numbers.empty()) return 1;

    data->min = data->max = data->numbers[0];

    for (size_t i = 0; i < data->numbers.size(); ++i) {
        int num = data->numbers[i];
        if (num < data->min) data->min = num;
        if (num > data->max) data->max = num;
        Sleep(7);
    }

    cout << "Min: " << data->min << ", Max: " << data->max << endl;
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    if (data->numbers.empty()) return 1;

    long long sum = 0;
    for (size_t i = 0; i < data->numbers.size(); ++i) {
        sum += data->numbers[i];
        Sleep(12);
    }

    data->average = (double)sum / data->numbers.size();
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

    for (size_t i = 0; i < data.numbers.size(); ++i) {
        if (data.numbers[i] == data.min || data.numbers[i] == data.max) {
            data.numbers[i] = (int)data.average;
        }
    }

    cout << "Modified array: ";
    for (size_t i = 0; i < data.numbers.size(); ++i) {
        cout << data.numbers[i] << " ";
    }
    cout << endl;

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    return 0;
}
