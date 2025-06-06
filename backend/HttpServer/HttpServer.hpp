#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    explicit HttpSession(tcp::socket socket);

    // Start the asynchronous operation
    void start();

private:
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void handle_request();
    void do_write();
    void on_write(boost::beast::error_code ec, std::size_t bytes_transferred, bool close);

    tcp::socket socket_;
    boost::beast::flat_buffer buffer_;

    http::request<http::string_body> req_;
    http::response<http::string_body> res_;
};

class HttpServer {
public:
    HttpServer(boost::asio::io_context& ioc, unsigned short port);
    void start();

private:
    void do_accept();
    void on_accept(boost::beast::error_code ec, tcp::socket socket);

    boost::asio::io_context& ioc_;
    tcp::acceptor acceptor_;
};
