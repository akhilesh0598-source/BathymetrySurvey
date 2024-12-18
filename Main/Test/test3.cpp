#include <boost/asio.hpp>
#include <iostream>

void task1() { std::cout << "Task 1 executed!" << std::endl; }
void task2() { std::cout << "Task 2 executed!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task1);
    boost::asio::post(io, task2);

    std::cout << "Processing one task at a time..." << std::endl;
    while (io.poll_one() > 0) {
        std::cout << "Processed one task!" << std::endl;
    }

    std::cout << "All tasks processed!" << std::endl;

    return 0;
}
