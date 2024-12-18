#include <boost/asio.hpp>
#include <iostream>
#include <thread>

namespace net = boost::asio;

void print_message(const std::string& msg) {
    std::cout << msg << " (Thread: " << std::this_thread::get_id() << ")\n";
}

int main() {
    net::io_context io;

    // Create a strand for thread safety
    auto strand = net::make_strand(io);

    // Dispatch a handler
    net::dispatch(strand, []() {
        print_message("Hello from dispatch!");
    });

    // Run the io_context in multiple threads
    std::thread t1([&io]() { io.run(); });
    std::thread t2([&io]() { io.run(); });

    t1.join();
    t2.join();

    return 0;
}
