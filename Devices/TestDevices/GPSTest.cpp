// #include "../GPS.hpp"

// int main()
// {
//     boost::asio::io_context io_context;
//     GPS gps(io_context,"/dev/GPS",9600);
//     io_context.run();
// }

#include <boost/asio.hpp>

int value = -1;

void my_task()
{
  value = 42;
}

int main() {
  boost::asio::io_context io_context;

  // Submit a function to the io_context.
  //boost::asio::post(io_context, my_task);

  // Run the io_context until it runs out of work.
  io_context.run();

  boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work(io_context.get_executor());

  // Submit a lambda object to the io_context.
  boost::asio::post(io_context,
      []()
      {
        my_task();
      });
    
  assert(value == 42);

}
