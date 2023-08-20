#include "CSGServer/websocket_session.hpp"

void fail(beast::error_code ec, char const* what) {
    LOGF << what << ": " << ec.message();
}

void do_session(websocket::stream<beast::tcp_stream>& ws, net::yield_context yield) {
    beast::error_code ec;

    // Set suggested timeout settings for the websocket
    ws.set_option(
            websocket::stream_base::timeout::suggested(
                    beast::role_type::server));

    // Set a decorator to change the Server of the handshake
    ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
            {
                res.set(http::field::server,
                        std::string(BOOST_BEAST_VERSION_STRING) +
                        " coolstory-gram-server");
            }));

    // Accept the websocket handshake
    ws.async_accept(yield[ec]);
    if(ec)
        return fail(ec, "accept");

    for(;;)
    {
        // This buffer will hold the incoming message
        beast::flat_buffer buffer;

        // Read a message
        ws.async_read(buffer, yield[ec]);

        // This indicates that the session was closed
        if(ec == websocket::error::closed) {
            LOGD << "Client closed the connection";
            break;
        }

        if(ec)
            return fail(ec, "read");

        LOGD << "(" << ws.next_layer().socket().remote_endpoint() << "): " << beast::make_printable(buffer.data());

        // Echo the message back
        ws.text(ws.got_text());
        ws.async_write(buffer.data(), yield[ec]);
        if(ec)
            return fail(ec, "write");
    }
}

void do_listen(net::io_context& ioc, tcp::endpoint endpoint, net::yield_context yield) {
    beast::error_code ec;

    // Open the acceptor
    tcp::acceptor acceptor(ioc);
    acceptor.open(endpoint.protocol(), ec);
    if(ec)
        return fail(ec, "open");

    // Allow address reuse
    acceptor.set_option(net::socket_base::reuse_address(true), ec);
    if(ec)
        return fail(ec, "set_option");

    // Bind to the server address
    acceptor.bind(endpoint, ec);
    if(ec)
        return fail(ec, "bind");

    // Start listening for connections
    acceptor.listen(net::socket_base::max_listen_connections, ec);
    if(ec)
        return fail(ec, "listen");

    for(;;)
    {
        tcp::socket socket(ioc);
        acceptor.async_accept(socket, yield[ec]);
        if(ec)
            fail(ec, "accept");
        else
            LOGD << "Connection accepted from " << socket.remote_endpoint();
            boost::asio::spawn(
                    acceptor.get_executor(),
                    std::bind(
                            &do_session,
                            websocket::stream<
                                    beast::tcp_stream>(std::move(socket)),
                            std::placeholders::_1));
    }
}

void run(net::ip::address address, unsigned short port, int threads) {
    // The io_context is required for all I/O
    net::io_context ioc(threads);

    // Spawn a listening port
    boost::asio::spawn(ioc,
                       std::bind(
                               &do_listen,
                               std::ref(ioc),
                               tcp::endpoint{address, port},
                               std::placeholders::_1));

    // Run the I/O service on the requested number of threads
    std::vector<std::thread> v;
    v.reserve(threads - 1);
    for(auto i = threads - 1; i > 0; --i)
        v.emplace_back(
                [&ioc]
                {
                    ioc.run();
                });
    ioc.run();
}
