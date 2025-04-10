#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

//two forks (mutexes)
std::mutex fork[2];  

void philosopher(int id) {
    //determine which forks this philosopher uses
    int left = id;
    int right = (id + 1) % 2;

    while (true) {
        //where the philosopher thinks
        std::cout << "Philosopher " << id << " is thinking...\n";

        std::this_thread::sleep_for(std::chrono::seconds(1));

        /**
         * pick up forks (lower-numbered first to avoid deadlock)
         * philosopher 0 picks up left fork first
         * philosopher 1 picks up right fork first
         */
        if (id == 0) {
            //left fork gets picked up first
            fork[left].lock();

            fork[right].lock();
        } else {
            //right fork gets picked up first
            fork[right].lock();

            fork[left].lock();
        }

        //where the philosopher eats. This is where if both forks are picked up, the philosopher will eat
        std::cout << "Philosopher " << id << " is eating...\n";

        std::this_thread::sleep_for(std::chrono::seconds(2));


        //the philosopher is done eating and puts down both forks
        fork[left].unlock();
        
        fork[right].unlock();
    }
}

int main() {
    //create two threads for the philosophers
    std::thread phil0(philosopher, 0);
    std::thread phil1(philosopher, 1);

    phil0.join();
    phil1.join();

    return 0;
}