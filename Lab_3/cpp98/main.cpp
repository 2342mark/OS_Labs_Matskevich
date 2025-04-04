#include <windows.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <atomic>

using namespace std;

CRITICAL_SECTION cs;
HANDLE startEvent;
vector<HANDLE> resumeEvents, stopEvents, doneEvents, threadHandles;
vector<vector<int>> markedIndices;
vector<int> sharedArray;
int arraySize;
atomic<int> waitingThreads(0);

DWORD WINAPI markerThread(LPVOID param) {
    int id = *(int*)param;
    srand(static_cast<unsigned int>(id));
    vector<int>& myMarks = markedIndices[id];

    WaitForSingleObject(startEvent, INFINITE);

    while (true) {
        bool marked = false;
        for (int i = 0; i < 10; ++i) {
            int idx = rand() % arraySize;

            EnterCriticalSection(&cs);
            if (sharedArray[idx] == 0) {
                sharedArray[idx] = id + 1;
                myMarks.push_back(idx);
                LeaveCriticalSection(&cs);
                Sleep(5);
                marked = true;
                break;
            }
            LeaveCriticalSection(&cs);
        }

        if (!marked) {
            EnterCriticalSection(&cs);
            cout << "Поток " << id << " не может продолжать. Помечено: " << myMarks.size() << endl;
            for (size_t idx = 0; idx < myMarks.size(); ++idx)
                cout << myMarks[idx] << " ";
            cout << "\n\n";
            LeaveCriticalSection(&cs);

            waitingThreads++;
            SetEvent(doneEvents[id]);

            HANDLE events[2] = { resumeEvents[id], stopEvents[id] };
            DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (waitResult == WAIT_OBJECT_0 + 1) {
                EnterCriticalSection(&cs);
                for (size_t idx = 0; idx < myMarks.size(); ++idx)
                    sharedArray[myMarks[idx]] = 0;
                LeaveCriticalSection(&cs);
                break;
            }

            waitingThreads--;
        }

        Sleep(5);
    }

    return 0;
}

void printArray() {
    EnterCriticalSection(&cs);
    cout << "Массив: ";
    for (size_t i = 0; i < sharedArray.size(); ++i)
        cout << sharedArray[i] << " ";
    cout << "\n";
    LeaveCriticalSection(&cs);
}

int main() {
    InitializeCriticalSection(&cs);

    cout << "Введите размер массива: ";
    cin >> arraySize;
    sharedArray.resize(arraySize, 0);

    cout << "Введите количество потоков-меток: ";
    int n;
    cin >> n;

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    resumeEvents.resize(n);
    stopEvents.resize(n);
    doneEvents.resize(n);
    markedIndices.resize(n);
    threadHandles.resize(n);

    vector<int> ids(n);
    vector<bool> active(n, true);

    for (int i = 0; i < n; ++i) {
        ids[i] = i;
        resumeEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        stopEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
        doneEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

        threadHandles[i] = CreateThread(NULL, 0, markerThread, &ids[i], 0, NULL);
    }

    SetEvent(startEvent);

    while (true) {
        cout << "Ожидание приостановки всех активных потоков...\n";

        while (waitingThreads < count(active.begin(), active.end(), true)) {
            Sleep(100);
        }
        printArray();

        cout << "Введите номер потока для завершения: ";
        int killId;
        cin >> killId;

        if (killId < 0 || killId >= n || !active[killId]) {
            cout << "Неверный номер потока. Попробуйте снова.\n";
            continue;
        }

        SetEvent(stopEvents[killId]);
        WaitForSingleObject(threadHandles[killId], INFINITE);
        active[killId] = false;

        printArray();

        for (int i = 0; i < n; ++i) {
            if (active[i])
                SetEvent(resumeEvents[i]);
        }

        if (count(active.begin(), active.end(), true) == 0)
            break;
    }

    DeleteCriticalSection(&cs);
    return 0;
}