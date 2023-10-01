// #include <iostream>
// #include <thread>
// #include <mutex>
// #include <chrono>

// const int NUM_PHILOSOPHERS = 5;
// std::mutex forks[NUM_PHILOSOPHERS];

// void philosopher(int id)
// {
//     int left_fork = id;
//     int right_fork = (id + 1) % NUM_PHILOSOPHERS;

//     while (true)
//     {
//         // Thinking
//         std::cout << "Philosopher " << id << " is thinking." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));

//         // Pick up forks
//         {
//             std::unique_lock<std::mutex> left_lock(forks[left_fork]);
//             std::unique_lock<std::mutex> right_lock(forks[right_fork]);

//             // Eating
//             std::cout << "Philosopher " << id << " is eating." << std::endl;
//             std::this_thread::sleep_for(std::chrono::seconds(1));
//         }

//         // Put down forks
//         std::cout << "Philosopher " << id << " is done eating. Putting down forks." << std::endl;
//         std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
// }

// int main()
// {
//     std::thread philosophers[NUM_PHILOSOPHERS];

//     // Start philosopher threads
//     for (int i = 0; i < NUM_PHILOSOPHERS; i++)
//     {
//         philosophers[i] = std::thread(philosopher, i);
//     }

//     // Join philosopher threads
//     for (int i = 0; i < NUM_PHILOSOPHERS; i++)
//     {
//         philosophers[i].join();
//     }

//     return 0;
// }

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <ctime>
#include <iomanip>

const int NUM_PHILOSOPHERS = 5;

class DiningPhilosophers
{
private:
    std::mutex forks[NUM_PHILOSOPHERS];
    std::condition_variable forksCV[NUM_PHILOSOPHERS];
    std::mutex printMutex;

public:
    void pickUpForks(int philosopherId)
    {
        std::unique_lock<std::mutex> leftLock(forks[philosopherId]);
        std::unique_lock<std::mutex> rightLock(forks[(philosopherId + 1) % NUM_PHILOSOPHERS]);

        printTimestamp("Philosopher " + std::to_string(philosopherId) + " picked up both forks.");
    }

    void putDownForks(int philosopherId)
    {
        forks[philosopherId].unlock();
        forks[(philosopherId + 1) % NUM_PHILOSOPHERS].unlock();

        forksCV[philosopherId].notify_one();
        forksCV[(philosopherId + 1) % NUM_PHILOSOPHERS].notify_one();
    }

    void printTimestamp(const std::string& message) 
    {
        std::lock_guard<std::mutex> lock(printMutex);

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::cout << std::put_time(std::localtime(&currentTime), "%F %T") << " - " << message << std::endl;
    }
};

DiningPhilosophers table;

void philosopher(int philosopherId)
{
    while (true)
    {
        // Thinking
        table.printTimestamp("Philosopher " + std::to_string(philosopherId) + " is thinking.");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Pick up forks
        table.pickUpForks(philosopherId);

        // Eating
        table.printTimestamp("Philosopher " + std::to_string(philosopherId) + " is eating.");
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Put down forks
        table.putDownForks(philosopherId);
    }
}

int main()
{
    std::thread philosophers[NUM_PHILOSOPHERS];

    // Start philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosophers[i] = std::thread(philosopher, i);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++)
    {
        philosophers[i].join();
    }

    return 0;
}
