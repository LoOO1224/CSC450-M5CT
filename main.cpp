
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

// Shared synchronization state for both counter threads.
struct CounterSync {
    std::mutex mutex;
    std::condition_variable ready;
    bool upFinished = false;
};

// Thread 1 counts upward from 0 through 20.
void countUp(CounterSync& sync)
{
    std::cout << "Thread 1 (count up): ";

    for (int value = 0; value <= 20; ++value) {
        std::cout << value << (value < 20 ? " " : "\n");
    }

    // Signal that the first counter has finished.
    {
        std::lock_guard<std::mutex> lock(sync.mutex);
        sync.upFinished = true;
    }

    sync.ready.notify_one();
}

// Thread 2 waits for Thread 1, then counts down to 0.
void countDown(CounterSync& sync)
{
    {
        std::unique_lock<std::mutex> lock(sync.mutex);

        // The predicate handles early notifications
        // and unexpected wake-ups safely.
        sync.ready.wait(lock, [&sync] {
            return sync.upFinished;
        });
    }

    std::cout << "Thread 2 (count down): ";

    for (int value = 20; value >= 0; --value) {
        std::cout << value << (value > 0 ? " " : "\n");
    }
}

int main()
{
    CounterSync sync;

    // Start two worker threads.
    std::thread first(countUp, std::ref(sync));
    std::thread second(countDown, std::ref(sync));

    // Wait for both threads before leaving main().
    first.join();
    second.join();

    std::cout << "Program finished successfully.\n";

    return 0;
}
