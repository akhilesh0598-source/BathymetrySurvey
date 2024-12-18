#include <boost/asio.hpp>
#include <iostream>
#include <thread>

void task() { 
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "Task executed!" << std::endl; 
    }

int main() {
    boost::asio::io_context io;
    //auto work = boost::asio::make_work_guard(io); // Keeps io_context alive
    boost::asio::post(io, task);
    std::thread t([&]() { io.run(); });

    for(int i=0;i<10000;i++)
        std::cout<<"message karna!";
   // work.reset(); // Allows io_context to stop after all tasks are complete

    t.join();

    return 0;
}
