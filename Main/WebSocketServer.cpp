#include "WebSocketServer.hpp"

WebSocketServer ::WebSocketServer()
{
}

WebSocketServer::session::session(tcp::socket &&socket) : ws_(std::move(socket))
{
    
}

void run()
{
    net::dispatch(ws_.get_executor(), beast::bind_front_handler(&session::on_run, shared_from_this()));
}

void on_run()
{
    // Set suggested timeout settings for the websocket
    ws_.set_option(websocket::stream_base::timeout::suggested(beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws_.set_option(websocket::stream_base::decorator(
        [](websocket::response_type &res)
        {
            res.set(http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
            // res.set("Access-Control-Allow-Origin", "*");
            // res.set("Access-Control-Allow-Credentials", "true");
        }));

    // Accept the websocket handshake
    ws_.async_accept(beast::bind_front_handler(&session::on_accept, shared_from_this()));
}

void on_accept(beast::error_code ec)
{
    if (ec)
        return fail(ec, "accept");

    // Wait for the first message from the client
    do_read();
}

void do_read()
{
    // Read a message into our buffer
    ws_.async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void on_read(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed by the client
    if (ec == websocket::error::closed)
        return;
    if (ec)
        return fail(ec, "read");

    // At this point, we have received the first message from the client
    // We can now start sending infinite data until the client disconnects.
    if (!first_message_received_)
    {
        first_message_received_ = true;

        start_sending();
    }
}

// Start the cycle of sending data infinitely
void start_sending()
{
    timer_ = std::make_shared<net::steady_timer>(ws_.get_executor());
    schedule_send();
}

void schedule_send()
{
    // Schedule the next send after some interval (50ms)
    timer_->expires_after(50ms);
    timer_->async_wait(beast::bind_front_handler(&session::on_timer, shared_from_this()));
}

void on_timer(beast::error_code ec)
{
    // Timer was canceled or error occurred
    if (ec)
        return;

    // Prepare the data to send
    buffer_.clear();

    std::string res = std::to_string(distance) + " " + std::to_string(confidence);
    auto prepared_buffer = buffer_.prepare(res.size());
    boost::asio::buffer_copy(prepared_buffer, boost::asio::buffer(res));
    buffer_.commit(res.size());

    // Write asynchronously
    ws_.async_write(buffer_.data(), beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void on_write(beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    if (ec)
    {
        // The client likely closed the connection or an error occurred.
        fail(ec, "write");
        return;
    }

    // Clear the buffer after writing
    buffer_.consume(buffer_.size());

    // Schedule the next send
    schedule_send();
}
}
;

class listener : public std::enable_shared_from_this<listener>
{
    net::io_context &ioc_;
    tcp::acceptor acceptor_;

public:
    listener(net::io_context &ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(ioc)
    {
        beast::error_code ec;

        acceptor_.open(endpoint.protocol(), ec);
        if (ec)
        {
            fail(ec, "open");
            return;
        }

        acceptor_.set_option(net::socket_base::reuse_address(true), ec);
        if (ec)
        {
            fail(ec, "set_option");
            return;
        }

        acceptor_.bind(endpoint, ec);
        if (ec)
        {
            fail(ec, "bind");
            return;
        }

        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec)
        {
            fail(ec, "listen");
            return;
        }
    }

    void run()
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&listener::on_accept, shared_from_this()));
    }

    void on_accept(beast::error_code ec, tcp::socket socket)
    {
        if (ec)
        {
            fail(ec, "accept");
        }
        else
        {
            // Create the session and run it
            std::make_shared<session>(std::move(socket))->run();
        }

        // Accept another connection
        do_accept();
    }
};
}
;