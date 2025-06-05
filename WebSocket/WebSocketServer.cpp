#include "WebSocketServer.hpp"

void fail(boost::beast::error_code ec, char const *what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

session::session(boost::asio::ip::tcp::socket &&socket) : ws_(std::move(socket))
{
}

void session::run()
{
    boost::asio::dispatch(ws_.get_executor(), boost::beast::bind_front_handler(&session::on_run, shared_from_this()));
}

void session::on_run()
{
    // Set suggested timeout settings for the websocket
    boost::beast::websocket::stream_base::timeout to =
        boost::beast::websocket::stream_base::timeout::suggested(boost::beast::role_type::server);

    to.keep_alive_pings = true;
   
    ws_.set_option(to);
    ws_.set_option(boost::beast::websocket::stream_base::decorator(
        [](boost::beast::websocket::response_type &res)
        {
            res.set(boost::beast::http::field::server, std::string(BOOST_BEAST_VERSION_STRING) + " websocket-server-async");
        }));

    // Accept the websocket handshake
    ws_.async_accept(boost::beast::bind_front_handler(&session::on_accept, shared_from_this()));
}

void session::on_accept(boost::beast::error_code ec)
{
    if (ec)
        return fail(ec, "accept");
    do_read();
}

void session::do_read()
{
    ws_.async_read(buffer_, boost::beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed by the client
    if (ec == boost::beast::websocket::error::closed)
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
    do_read();
}

// Start the cycle of sending data infinitely
void session::start_sending()
{
    timer_ = std::make_shared<boost::asio::steady_timer>(ws_.get_executor());
    schedule_send();
}

void session::schedule_send()
{
    using namespace std::chrono_literals;
    // Schedule the next send after some interval (50ms)
    timer_->expires_after(50ms);
    timer_->async_wait(boost::beast::bind_front_handler(&session::on_timer, shared_from_this()));
}

void session::on_timer(boost::beast::error_code ec)
{
    // Timer was canceled or error occurred
    if (ec)
        return;

    // Prepare the data to send
    buffer_.clear();

    // std::string res = "{\"distance\":\"" + std::to_string(pingDeviceDistance) + "\",\"confidence\" : \"" + std::to_string(pingDeviceConfidence) + "\",\"gpsDateTime\":\"" + gpsDateTime + "\",\"gpsLatitude\":\"" + std::to_string(gpsLatitude) + "\",\"gpsLongitude\":\"" + std::to_string(gpsLongitude) + "\"}";
    // auto prepared_buffer = buffer_.prepare(res.size());
    // boost::asio::buffer_copy(prepared_buffer, boost::asio::buffer(res));
    // buffer_.commit(res.size());

    // Write asynchronously
    ws_.async_write(buffer_.data(), boost::beast::bind_front_handler(&session::on_write, shared_from_this()));
}

void session::on_write(boost::beast::error_code ec, std::size_t bytes_transferred)
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

listener::listener(boost::asio::io_context &ioc, boost::asio::ip::tcp::endpoint endpoint) : ioc_(ioc), acceptor_(ioc)
{
    boost::beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec)
    {
        fail(ec, "open");
        return;
    }

    acceptor_.set_option(boost::asio::socket_base::reuse_address(true), ec);
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

    acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
    if (ec)
    {
        fail(ec, "listen");
        return;
    }
}

void listener::run()
{
    do_accept();
}

void listener::do_accept()
{
    acceptor_.async_accept(boost::asio::make_strand(ioc_), boost::beast::bind_front_handler(&listener::on_accept, shared_from_this()));
}

void listener::on_accept(boost::beast::error_code ec, boost::asio::ip::tcp::socket socket)
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
};
