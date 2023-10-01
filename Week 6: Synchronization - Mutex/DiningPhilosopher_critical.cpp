#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

//number of philosopher
#define nr_philosopher 5

std::mutex chopsticks[nr_philosopher];

void philosopher(int id)
{
    int left_chop = id;
    int right_chop = (id + 1) % nr_philosopher;

    while (true)
    {
        std::cout << "Philosopher " << id << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        // Take chopsticks in critical section
        chopsticks[left_chop].lock();
        chopsticks[right_chop].lock();
        std::cout << "Chopstick " << left_chop<< " and "<<right_chop << " is used by philosopher " <<id<< std::endl;
        std::cout << "Philosopher " << id << " is eating." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        
        // Put down chopsticks
        chopsticks[left_chop].unlock();
        chopsticks[right_chop].unlock();
        std::cout << "Chopstick " << left_chop<< " and "<<right_chop << " is free." << std::endl;
    }
}

int main()
{
    std::thread philosophers[nr_philosopher];

    for (int i = 0; i < nr_philosopher; i++)
    {
        philosophers[i] = std::thread(philosopher, i);
    }
    
    for (int i = 0; i < nr_philosopher; i++)
    {
        philosophers[i].join();
    }

    return 0;
}
