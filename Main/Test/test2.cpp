#include <boost/asio.hpp>
#include <iostream>

void task1() { std::cout << "Task 1 executed!" << std::endl; }
void task2() { std::cout << "Task 2 executed!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task1);
    boost::asio::post(io, task2);

    std::cout << "Polling handlers without blocking..." << std::endl;
    while (io.poll() > 0) {
        std::cout << "Processed some tasks!" << std::endl;
    }

    std::cout << "No more ready tasks!" << std::endl;

    return 0;
}
