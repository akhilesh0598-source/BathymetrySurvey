#include <boost/asio.hpp>
#include <iostream>

void task() { std::cout << "Task executed!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task);
    io.run(); // Processes the task

    std::cout << "Restarting io_context..." << std::endl;
    //io.restart();

    boost::asio::post(io, task);
    io.run(); // Processes the new task
    std::cout << "Tasks completed after restart!" << std::endl;

    return 0;
}
