#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "json.hpp"

using json = nlohmann::json;
using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
using websocket_stream = websocket::stream<tcp::socket>;

int main() {
    boost::asio::io_context ioc;
    tcp::resolver resolver(ioc);
    
    std::cout << "[Client] resolve with localhost 8080\n";
    auto const results = resolver.resolve("localhost", "8080");
    websocket_stream ws(ioc);

    std::cout << "[Client] connect with localhost\n";
    auto ep = boost::asio::connect(ws.next_layer(), results);
    
    std::cout << "[Client] Handshake with localhost\n";
    ws.handshake("localhost", "/");

    std::cout << "[Client] Writing Heartbeat Period Request\n";
    ws.write(boost::asio::buffer(json{{}}.dump()));
    std::cout << "[Client] Wrote Heartbeat Period Request\n";

    boost::beast::multi_buffer buffer;
    
    ws.read(buffer);
    std::cout << "[Client] Got Heartbeat Period Response\n";

    auto period = json::parse(boost::beast::buffers_to_string(buffer.data()))["period"];

    for (;;) {
        auto period_in_seconds = std::chrono::seconds(period);

        std::cout << "[Client] Sleeping for "  << period_in_seconds "\n";
        std::this_thread::sleep_for(period_in_seconds);
        std::cout << "[Client] Done Sleeping\n";

        std::cout << "[Client] Writing Heartbeat Request \n";
        ws.write(boost::asio::buffer(json{{"client_id", "client_number_1"}}.dump()));
        std::cout << "[Client] Wrote Heartbeat Request\n";

        ws.read(buffer);
        std::cout << "[Client] Got Heartbeat Response\n";
    }
}
