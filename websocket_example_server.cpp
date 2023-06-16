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
    std::cout << "[Server] tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});\n";
    tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

    for (;;) {
        std::cout << "[Server] tcp::socket socket(ioc);\n";
        tcp::socket socket(ioc);

        std::cout << "[Server] acceptor.accept(socket)\n";
        acceptor.accept(socket);

        std::cout << "[Server] std::thread([socket = std::move(socket)]()\n";
        std::thread([socket = std::move(socket)]() mutable {

            std::cout << "[Server] websocket_stream ws(std::move(socket));\n";
            websocket_stream ws(std::move(socket));

            std::cout << "[Server] ws.accept();\n";
            ws.accept();

            for (;;) {
                boost::beast::multi_buffer buffer;
                std::cout << "[Server] ws.read(buffer);\n";
                ws.read(buffer);

                std::cout << "[Server] auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));\n";

                auto msg = json::parse(boost::beast::buffers_to_string(buffer.data()));
                if (msg.empty()) {
                    // HeartbeatPeriodRequest
                    std::cout << "[Server] write HeartbeatPeriodRequest\n";
                    ws.write(boost::asio::buffer(json{{"period", 100}}.dump()));
                } else {
                    // HeartbeatRequest
                    std::cout << "[Server] write HeartbeatRequest\n";
                    ws.write(boost::asio::buffer(json{{}}.dump()));
                }
            }
        }).detach();
    }
}
