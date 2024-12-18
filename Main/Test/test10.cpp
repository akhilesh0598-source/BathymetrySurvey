#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <vector>

namespace net = boost::asio;

int counter = 0; // Shared data

void increment_counter(int id) {
    ++counter; // Safely increment shared data
    std::cout << "Handler ID: " << id
              << ", Counter Value: " << counter
              << ", Thread: " << std::this_thread::get_id() << std::endl;
}

int main() {
    net::io_context io;

    // Create a strand to serialize access to shared data
    auto strand = net::make_strand(io);

    // Post multiple tasks to increment the counter
    for (int i = 0; i < 5; ++i) {
        net::post(io, [i]() {
            increment_counter(i); // Safely modifies shared data
        });
    }

    // Launch multiple threads to process the tasks
    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&io]() { io.run(); });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final Counter Value: " << counter << std::endl;

    return 0;
}
