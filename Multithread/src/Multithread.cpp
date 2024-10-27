#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

// Mutex and condition variable for synchronization
std::mutex mtx;
std::condition_variable cv;
bool isThreadOneDone = false; // Flag to signal thread completion

void countUp() {
    for (int i = 0; i <= 20; ++i) {
        std::lock_guard<std::mutex> lock(mtx); // Lock for thread-safe output
        std::cout << "Thread 1 (Count Up): " << i << std::endl;
    }
    // Notify the other thread to start after count-up is complete
    {
        std::lock_guard<std::mutex> lock(mtx);
        isThreadOneDone = true;
    }
    cv.notify_one();
}

void countDown() {
    // Wait until thread one completes its task
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return isThreadOneDone; });

    // Count down from 20 to 0
    for (int i = 20; i >= 0; --i) {
        std::cout << "Thread 2 (Count Down): " << i << std::endl;
    }
}

int main() {
    // Creating two threads
    std::thread thread1(countUp);
    std::thread thread2(countDown);

    // Join threads to main thread
    thread1.join();
    thread2.join();

    return 0;
}
