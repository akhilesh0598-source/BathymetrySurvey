#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <string>
#include<iostream>

#include "../GlobalVariables/GlobalDevicesData.hpp"

class WebSocketSession : public std::enable_shared_from_this<WebSocketSession> {
public:
    explicit WebSocketSession(boost::asio::ip::tcp::socket socket);
    void start();

private:
    boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
    boost::beast::flat_buffer buffer_;
    boost::asio::steady_timer timer_;
    bool first_message_received_ = false;

    void start_sending();
};

class WebSocketServer {
public:
    WebSocketServer(boost::asio::io_context& ioc, uint16_t port);
    void start();

private:
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
};
