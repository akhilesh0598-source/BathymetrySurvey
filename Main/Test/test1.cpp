#include <boost/asio.hpp>
#include <iostream>
#include<thread>
#include<chrono>

void task1() { 
    std::cout << "Task 1 executed!" << std::endl;
    std::chrono::milliseconds mili(1000);
    std::this_thread::sleep_for(mili);
    return;
 }
void task2() { std::cout << "Task 2 executed!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task1);
    boost::asio::post(io, task2);

    std::cout << "Starting io_context.run()" << std::endl;
    io.run(); // Processes all tasks and blocks until done
    std::cout << "All tasks completed!" << std::endl;

    return 0;
}
