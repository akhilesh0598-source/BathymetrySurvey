#include "HttpServer.hpp"
#include <boost/beast/version.hpp>
#include <iostream>

LogManager logManager;

HttpSession::HttpSession(tcp::socket socket)
    : socket_(std::move(socket)) {}

void HttpSession::start() {
    do_read();
}

void HttpSession::do_read() {
    auto self = shared_from_this();
    http::async_read(socket_, buffer_, req_,
                     [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
                         self->on_read(ec, bytes_transferred);
                     });
}

void HttpSession::on_read(boost::beast::error_code ec, std::size_t) {
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

void HttpSession::handle_request() {
    res_.version(req_.version());
    res_.keep_alive(req_.keep_alive());
    res_.set(http::field::server, "SimpleHttpServer");
    res_.set(http::field::access_control_allow_origin, "*");
    res_.set(http::field::access_control_allow_methods, "GET, POST, OPTIONS");
    res_.set(http::field::access_control_allow_headers, "Content-Type");

    std::string path = std::string(req_.target());

    if (req_.method() == http::verb::get) {
        handle_get(path);
    } else if (req_.method() == http::verb::post) {
        handle_post(path);
    } else {
        res_.result(http::status::bad_request);
        res_.set(http::field::content_type, "text/plain");
        res_.body() = "Unsupported HTTP method.";
    }

    res_.prepare_payload();
    do_write();
}

std::string HttpSession::extractQueryParam(const std::string& target, const std::string& key) {
    auto pos = target.find('?');
    if (pos == std::string::npos) return "";

    std::string query = target.substr(pos + 1);
    std::istringstream stream(query);
    std::string token;
    while (std::getline(stream, token, '&')) {
        auto eq = token.find('=');
        if (eq != std::string::npos) {
            std::string k = token.substr(0, eq);
            std::string v = token.substr(eq + 1);
            if (k == key) return v;
        }
    }
    return "";
}

void HttpSession::handle_get(const std::string& path) {
    if (path == "/") {
        res_.result(http::status::ok);
        res_.set(http::field::content_type, "text/plain");
        res_.body() = "Welcome to the root GET!";
    }
    else if (path.rfind("/startLogging", 0) == 0) {
        std::string clientId = extractQueryParam(path, "clientId");
        if (clientId.empty()) {
            res_.result(http::status::bad_request);
            res_.body() = "Missing clientId";
        } else {
            logManager.startLogging(clientId);
            res_.result(http::status::ok);
            res_.set(http::field::content_type, "application/json");
            res_.body() = "{\"status\": \"Logging started for " + clientId + "\"}";
        }
    }
    else if (path.rfind("/stopLogging", 0) == 0) {
        std::string clientId = extractQueryParam(path, "clientId");
        if (clientId.empty()) {
            res_.result(http::status::bad_request);
            res_.body() = "Missing clientId";
        } else {
            logManager.stopLogging(clientId);
            res_.result(http::status::ok);
            res_.set(http::field::content_type, "application/json");
            res_.body() = "{\"status\": \"Logging stopped for " + clientId + "\"}";
        }
    }
    else if (path.rfind("/download", 0) == 0) {
        std::string clientId = extractQueryParam(path, "clientId");
        std::string filePath = logManager.getLastLogFilePath(clientId);

        if (clientId.empty() || filePath.empty() || !std::ifstream(filePath)) {
            res_.result(http::status::not_found);
            res_.set(http::field::content_type, "text/plain");
            res_.body() = "Log file not found.";
        } else {
            std::ifstream file(filePath, std::ios::binary);
            std::ostringstream ss;
            ss << file.rdbuf();
            std::string fileContent = ss.str();

            res_.result(http::status::ok);
            res_.set(http::field::content_type, "text/csv");
            res_.set(http::field::content_disposition,
                     "attachment; filename=\"" + fs::path(filePath).filename().string() + "\"");
            res_.body() = std::move(fileContent);
        }
    }
    else {
        res_.result(http::status::not_found);
        res_.set(http::field::content_type, "text/plain");
        res_.body() = "404 Not Found";
    }
}

void HttpSession::handle_post(const std::string& path) {
    res_.result(http::status::not_found);
    res_.set(http::field::content_type, "text/plain");
    res_.body() = "404 Not Found";
}

void HttpSession::do_write() {
    auto self = shared_from_this();
    http::async_write(socket_, res_,
                      [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
                          self->on_write(ec, bytes_transferred, !self->res_.keep_alive());
                      });
}

void HttpSession::on_write(boost::beast::error_code ec, std::size_t, bool close) {
    if (ec) {
        std::cerr << "Write error: " << ec.message() << "\n";
        return;
    }
    if (close) {
        boost::beast::error_code ignored_ec;
        socket_.shutdown(tcp::socket::shutdown_send, ignored_ec);
        return;
    }
    req_ = {};
    do_read();
}

HttpServer::HttpServer(boost::asio::io_context& ioc, unsigned short port)
    : ioc_(ioc), acceptor_(ioc) {
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

void HttpServer::start() {
    do_accept();
}

void HttpServer::do_accept() {
    acceptor_.async_accept(
        [this](boost::beast::error_code ec, tcp::socket socket) {
            on_accept(ec, std::move(socket));
        });
}

void HttpServer::on_accept(boost::beast::error_code ec, tcp::socket socket) {
    if (!ec) {
        std::make_shared<HttpSession>(std::move(socket))->start();
    } else {
        std::cerr << "Accept error: " << ec.message() << "\n";
    }
    do_accept();
}
