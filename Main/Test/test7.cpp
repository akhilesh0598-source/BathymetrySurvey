#include <boost/asio.hpp>
#include <iostream>

void task() { std::cout << "Task executed asynchronously!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task);
    io.run(); // Executes the posted task

    return 0;
}
