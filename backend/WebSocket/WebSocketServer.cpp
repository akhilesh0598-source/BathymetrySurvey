#include "WebSocketServer.hpp"

using namespace boost;
using tcp = asio::ip::tcp;
namespace websocket = beast::websocket;

// WebSocketSession Implementation
WebSocketSession::WebSocketSession(tcp::socket socket)
    : ws_(std::move(socket)), timer_(ws_.get_executor()) {}

void WebSocketSession::start()
{
    auto self = shared_from_this();
    ws_.async_accept([self](boost::beast::error_code ec)
                     {
                         if (ec)
                         {
                             std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
                             return;
                         }
                         self->first_message_received_ = true; // Automatically start sending
                         self->start_sending();                // Start sending without waiting for client message
                     });
}

void WebSocketSession::start_sending()
{
    timer_.expires_after(std::chrono::seconds(1));
    timer_.async_wait([self = shared_from_this()](beast::error_code ec)
                      {
        if (ec || !self->first_message_received_) return;

        std::stringstream ss;
        {
            boost::mutex::scoped_lock lock1(sonar_mutex);
            boost::mutex::scoped_lock lock2(gps_mutex);
            ss << "{"
   << "\"distance\":" << sonarDistance << ", "
   << "\"confidence\":" << sonarConfidence << ", "
   << "\"latitude\":" << gpsLatitude << ", "
   << "\"longitude\":" << gpsLongitude << ", "
   << "\"gpsDateTime\":\"" << gpsDateTime << "\""
   << "}";
        }

        self->ws_.async_write(asio::buffer(ss.str()), [self](beast::error_code ec, std::size_t) {
            if (!ec) self->start_sending();
        }); });
}

// WebSocketServer Implementation
WebSocketServer::WebSocketServer(asio::io_context &ioc, uint16_t port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {}

void WebSocketServer::start()
{
    acceptor_.async_accept(socket_, [this](beast::error_code ec)
                           {
                               if (!ec)
                                   std::make_shared<WebSocketSession>(std::move(socket_))->start();
                               start(); // accept next
                           });
}
