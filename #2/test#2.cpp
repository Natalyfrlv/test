#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

// Общее число операций записи
const int NUM_WRITES = 50;

// Мьютекс для синхронизации доступа к переменной
std::mutex sharedMutex;

// Условная переменная для сигнализации о готовности данных
std::condition_variable dataReady;
// Переменная, к которой будут обращаться потоки
int sharedVariable = 0;

// Функция для записи данных в переменную
void writerFunction() {
    for (int i = 0; i < NUM_WRITES; ++i) {
        {
            std::lock_guard<std::mutex> lock(sharedMutex);
            sharedVariable = i + 1; 
            std::cout << "Writer: wrote value " << sharedVariable << "\n";
        }

        dataReady.notify_one(); // Подаем сигнал, что данные записаны
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

// Функция для чтения данных из переменной
void readerFunction() {
    while (true) {
        std::unique_lock<std::mutex> lock(sharedMutex);

        dataReady.wait(lock, []() { return sharedVariable > 0; });
        std::cout << "Reader: read value " << sharedVariable << "\n";
        if (sharedVariable >= NUM_WRITES) {
            break;
        }
        sharedVariable = 0;
    }
}

int main() {
    std::thread writerThread(writerFunction);
    std::thread readerThread(readerFunction);
    writerThread.join();
    readerThread.join();

    return 0;
}