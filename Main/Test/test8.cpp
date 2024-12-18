#include <boost/asio.hpp>
#include <iostream>

void task() { std::cout << "Task executed immediately if possible!" << std::endl; }

int main() {
    boost::asio::io_context io;

    boost::asio::dispatch(io, task); // Executes synchronously if in the same thread
    io.run();
    return 0;
}
