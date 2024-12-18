#include <boost/asio/io_context.hpp>
#include<boost/asio/ip/tcp.hpp>
#include<boost/beast/websocket.hpp>
#include<boost/beast/websocket

class session
{
    private:
        boost::asio::ip::tcp::resolver resolver;
    boost::beast::websocket ::stream<boost::beast:tcp_str

    public:
    explicit session(boost::asio::io_context &io_context):resolver(io_context)
    {

    }
    
};

int main(int argc,char args [])
{
    const char host[] ="127.0.0.1";
    const char port[]="5001";

    boost::asio::io_context io_context;

    return 0;
}
