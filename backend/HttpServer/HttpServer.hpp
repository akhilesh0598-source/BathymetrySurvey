#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "../LogDevicesData/LogDevicesData.hpp"

using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;
namespace fs = std::filesystem;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
public:
    explicit HttpSession(tcp::socket socket);
    void start();

private:
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
    void handle_request();
    void do_write();
    void on_write(boost::beast::error_code ec, std::size_t bytes_transferred, bool close);
    void handle_get(const std::string& path);
    void handle_post(const std::string& path);
    std::string extractQueryParam(const std::string& target, const std::string& key);

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
