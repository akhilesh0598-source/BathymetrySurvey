// WebSocketServer.hpp
#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread/mutex.hpp>
#include <memory>
#include <string>
#include <sstream>

#include "../GlobalVariables/GlobalDevicesData.hpp"

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    explicit WebSocketSession(boost::asio::ip::tcp::socket socket);
    void start();

private:
    void start_sending();
    void read_messages(); // Optional: if you want to receive from client

    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    boost::asio::steady_timer timer_;
    boost::beast::flat_buffer buffer_; // for reading
    bool first_message_received_ = false;
    bool is_open_ = true;
};

class WebSocketServer {
public:
    WebSocketServer(boost::asio::io_context& ioc, uint16_t port);
    void start();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};
