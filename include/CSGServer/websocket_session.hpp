#pragma once

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <logger/log.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/spawn.hpp>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

void fail(beast::error_code ec, char const* what);
void do_session(websocket::stream<beast::tcp_stream>& ws, net::yield_context yield);
void do_listen(net::io_context& ioc, tcp::endpoint endpoint, net::yield_context yield);
void run(net::ip::address address, unsigned short port, int threads);