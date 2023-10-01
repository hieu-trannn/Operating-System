#include <iostream>
#include <thread>
#include <mutex>
#include <ctime>

const int NUM_PHILOSOPHERS = 5;
std::mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id)
{
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    while (true)
    {
        // Thinking
        std::cout << "Philosopher " << id << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Pick up forks
        forks[left_fork].lock();
        forks[right_fork].lock();

        // Eating
        std::cout << "Philosopher " << id << " is eating." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        // Put down forks
        forks[left_fork].unlock();
        forks[right_fork].unlock();
    }
}

int main()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
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
    // std::cout<<"<==================Second:"<<
    return 0;
}
