#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/steady_timer.hpp>
#include<iostream>

// namespace beast = boost::beast;
// namespace http = beast::http;
// namespace websocket = beast::websocket;
// namespace net = boost::asio;
// using tcp = boost::asio::ip::tcp;
// using namespace std::chrono_literals;

void fail(boost::beast::error_code ec, char const *what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

class WebSocketServer
{
public:

    int *ping_device_distance_value;
    int *ping_device_confidence_value;
    
    WebSocketServer(std::string ip_address,uint port,uint number_of_threads);

    void set_ping_device_variable(int *distance,int *confidence);

    class session : public std::enable_shared_from_this<session>
    {

    private:

        boost::beast::websocket::stream<boost::beast::tcp_stream> ws_;
        boost::beast::flat_buffer buffer_;
        // Timer for repeated sends
        std::shared_ptr<boost::asio::steady_timer> timer_;
        bool first_message_received_ = false;

    public:

        explicit session(boost::asio::ip::tcp::socket &&socket);

        void run();

        void on_run();

        void on_accept(boost::beast::error_code ec);

        void do_read();

        void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);

        // Start the cycle of sending data infinitely
        void start_sending();

        void schedule_send();

        void on_timer(boost::beast::error_code ec);

        void on_write(boost::beast::error_code ec, std::size_t bytes_transferred);
    
    };

    class listener : public std::enable_shared_from_this<listener>
    {

    private:
        boost::asio::io_context &ioc_;
        boost::asio::ip::tcp::acceptor acceptor_;

    public:
        listener(boost::asio::io_context &ioc, boost::asio::ip::tcp::endpoint endpoint);

        void run();

    private:
        void do_accept();

        void on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket);
    
    };
};