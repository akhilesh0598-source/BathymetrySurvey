// WebSocketServer.cpp
#include "WebSocketServer.hpp"
#include <iostream>

using namespace boost;
namespace websocket = beast::websocket;
namespace asio = boost::asio;
using tcp = asio::ip::tcp;

// --- WebSocketSession Implementation ---
WebSocketSession::WebSocketSession(tcp::socket socket)
    : ws_(std::move(socket)), timer_(ws_.get_executor()) {}

void WebSocketSession::start()
{
    auto self = shared_from_this();
    ws_.async_accept([self](beast::error_code ec)
                     {
                         if (ec)
                         {
                             std::cerr << "WebSocket accept error: " << ec.message() << std::endl;
                             return;
                         }

                         self->first_message_received_ = true;
                         self->start_sending();
                         // self->read_messages(); // Optional: allow client messages
                     });
}

void WebSocketSession::start_sending()
{
    if (!is_open_)
        return;

    timer_.expires_after(std::chrono::seconds(1));
    auto self = shared_from_this();
    timer_.async_wait([self](beast::error_code ec)
                      {
                          if (ec || !self->is_open_ || !self->first_message_received_)
                              return;

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

                          self->ws_.async_write(asio::buffer(ss.str()), [self](beast::error_code ec, std::size_t)
                                                {
                                                    if (ec) {
                                                        if (ec == websocket::error::closed || ec == asio::error::operation_aborted) {
                                                            std::cout << "WebSocket closed by client.\n";
                                                        } else {
                                                            std::cerr << "Write error: " << ec.message() << std::endl;
                                                        }
                                                        self->is_open_ = false;
                                                        return;
                                                    }
    if (self->is_open_) {
        self->start_sending(); // Continue sending only if still open
    } }); });
}

// Optional: Handle incoming client messages like "STOP"
void WebSocketSession::read_messages()
{
    auto self = shared_from_this();
    ws_.async_read(buffer_, [self](beast::error_code ec, std::size_t)
                   {
        if (!ec) {
            std::string msg = beast::buffers_to_string(self->buffer_.data());
            if (msg == "STOP") {
                self->is_open_ = false;
                self->ws_.close(websocket::close_code::normal);
                std::cout << "Session stopped by client.\n";
            } else {
                // handle other commands if needed
                self->read_messages(); // listen again
            }
            self->buffer_.consume(self->buffer_.size());
        } });
}

// --- WebSocketServer Implementation ---
WebSocketServer::WebSocketServer(asio::io_context &ioc, uint16_t port)
    : acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), socket_(ioc) {}

void WebSocketServer::start()
{
    std::cout << "WebSocket server is listening on port " 
              << acceptor_.local_endpoint().port() << std::endl;

    acceptor_.async_accept(socket_, [this](beast::error_code ec)
                           {
                               if (!ec)
                               {
                                    std::cout << "Accepted new WebSocket connection\n";
                                    std::make_shared<WebSocketSession>(std::move(socket_))->start();
                               }
                               else
                               {
                                   std::cerr << "Accept error: " << ec.message() << std::endl;
                               }
                               start(); // continue accepting
                           });
}

void WebSocketServer::stop()
{
    boost::system::error_code ec;

    // Cancel any pending async_accept operations (optional but good practice)
    acceptor_.cancel(ec);
    if (ec) {
        std::cerr << "WebSocketServer cancel error: " << ec.message() << "\n";
    }

    // Close the acceptor to stop new connections
    acceptor_.close(ec);
    if (ec) {
        std::cerr << "WebSocketServer close error: " << ec.message() << "\n";
    }

    std::cout << "WebSocket server stopped.\n";
}

