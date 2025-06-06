#include "HttpServer.hpp"
#include <boost/beast/version.hpp>
#include <iostream>

// HttpSession implementation

HttpSession::HttpSession(tcp::socket socket)
    : socket_(std::move(socket))
{
}

void HttpSession::start()
{
    do_read();
}

void HttpSession::do_read()
{
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, req_,
        [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
            self->on_read(ec, bytes_transferred);
        });
}

void HttpSession::on_read(boost::beast::error_code ec, std::size_t)
{
    if (ec == http::error::end_of_stream) {
        socket_.shutdown(tcp::socket::shutdown_send, ec);
        return;
    }
    if (ec) {
        std::cerr << "Read error: " << ec.message() << "\n";
        return;
    }

    handle_request();
}

void HttpSession::handle_request()
{
    // For simplicity, respond with a fixed string
    res_.version(req_.version());
    res_.keep_alive(req_.keep_alive());
    res_.result(http::status::ok);
    res_.set(http::field::server, "SimpleHttpServer");
    res_.set(http::field::content_type, "text/plain");
    res_.body() = "Hello from HttpServer!";
    res_.prepare_payload();

    do_write();
}

void HttpSession::do_write()
{
    auto self = shared_from_this();
    http::async_write(socket_, res_,
        [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
            self->on_write(ec, bytes_transferred, !self->res_.keep_alive());
        });
}

void HttpSession::on_write(boost::beast::error_code ec, std::size_t, bool close)
{
    if (ec) {
        std::cerr << "Write error: " << ec.message() << "\n";
        return;
    }
    if (close) {
        boost::beast::error_code ignored_ec;
        socket_.shutdown(tcp::socket::shutdown_send, ignored_ec);
        return;
    }
    // Clear request and read another
    req_ = {};
    do_read();
}

// HttpServer implementation

HttpServer::HttpServer(boost::asio::io_context& ioc, unsigned short port)
    : ioc_(ioc)
    , acceptor_(ioc)
{
    boost::beast::error_code ec;

    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
        std::cerr << "Open error: " << ec.message() << "\n";
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec) {
        std::cerr << "Set option error: " << ec.message() << "\n";
        return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
        std::cerr << "Bind error: " << ec.message() << "\n";
        return;
    }

    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec) {
        std::cerr << "Listen error: " << ec.message() << "\n";
        return;
    }
}

void HttpServer::start()
{
    do_accept();
}

void HttpServer::do_accept()
{
    acceptor_.async_accept(
        [this](boost::beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        });
}

void HttpServer::on_accept(boost::beast::error_code ec, tcp::socket socket)
{
    if (!ec) {
        // Create and start new session for accepted socket
        std::make_shared<HttpSession>(std::move(socket))->start();
    } else {
        std::cerr << "Accept error: " << ec.message() << "\n";
    }

    // Accept next connection
    do_accept();
}
