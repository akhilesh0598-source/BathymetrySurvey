#include <boost/asio.hpp>
#include <iostream>

void delayed_task(boost::asio::steady_timer& timer) {
    std::cout << "Delayed Task Executed!" << std::endl;
}

int main() {
    boost::asio::io_context io;
    boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(1));
    boost::asio::steady_timer timer2(io, boost::asio::chrono::seconds(1));

    timer.async_wait([&](const boost::system::error_code&) {
        delayed_task(timer);
    });
    timer2.async_wait([&](const boost::system::error_code&) {
        delayed_task(timer);
    });

    std::cout << "Running one task at a time (blocking)..." << std::endl;
    while (io.run_one() > 0) {
        std::cout << "Processed one handler!" << std::endl;
    }

    std::cout << "All handlers processed!" << std::endl;

    return 0;
}
