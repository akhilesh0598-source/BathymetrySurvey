#include <boost/asio.hpp>
#include <iostream>

void task() { std::cout << "Task executed!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::post(io, task);
    io.stop(); // Stop the io_context

    std::cout << "Calling run after stop..." << std::endl;
    io.run(); // No tasks will be executed
    std::cout << "io_context stopped, no handlers executed!" << std::endl;

    return 0;
}
