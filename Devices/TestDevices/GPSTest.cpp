#include "../GPS.hpp"

int main()
{
    boost::asio::io_context io_context;
    GPS gps(io_context,"/dev/GPS",9600);
    io_context.run();
}
