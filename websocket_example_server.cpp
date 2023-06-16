#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio/io_context.hpp>
#include <thread>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
using websocket_stream = websocket::stream<tcp::socket>;

int main() {
    boost::asio::io_context ioc;
    tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

    for (;;) {
        tcp::socket socket(ioc);
        acceptor.accept(socket);
        std::thread([socket = std::move(socket)]() mutable {
            websocket_stream ws(std::move(socket));
            ws.accept();

            for (;;) {
                boost::beast::multi_buffer buffer;
                ws.read(buffer);

                auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));
                if (msg.empty()) {
                    // HeartbeatPeriodRequest
                    ws.write(boost::asio::buffer(json{{"period", 100}}.dump()));
                } else {
                    // HeartbeatRequest
                    ws.write(boost::asio::buffer(json{{}}.dump()));
                }
            }
        }).detach();
    }
}
