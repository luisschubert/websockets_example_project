#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <chrono>
#include "json.hpp"

using json = nlohmann::json;
using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;
using websocket_stream = websocket::stream<tcp::socket>;

int main() {
    boost::asio::io_context ioc;
    tcp::resolver resolver(ioc);
    auto const results = resolver.resolve("localhost", "8080");
    websocket_stream ws(ioc);
    auto ep = boost::asio::connect(ws.next_layer(), results);
    ws.handshake("localhost", "/");
    ws.write(boost::asio::buffer(json{{}}.dump()));

    boost::beast::multi_buffer buffer;
    ws.read(buffer);
    auto period = json::parse(boost::beast::buffers_to_string(buffer.data()))["period"];

    for (;;) {
        std::this_thread::sleep_for(std::chrono::seconds(period));
        ws.write(boost::asio::buffer(json{{"client_id", "client_number_1"}}.dump()));
        ws.read(buffer);
    }
}
