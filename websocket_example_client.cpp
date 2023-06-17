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


void print_different_json_formats()
{
    auto msg = json::parse(json{{"a", 1}}.dump());
    std::cout << "json::parse(json({\"a\", 1}).dump())" << std::endl;
    std::cout << msg                            << std::endl;
    std::cout << msg.empty()                    << std::endl;
    std::cout << msg["a"]                       << std::endl << std::endl;

    json empty_object_implicit = json({});
    std::cout << "json({});"                    << std::endl;
    std::cout << empty_object_implicit          << std::endl;
    std::cout << empty_object_implicit.empty()  << std::endl << std::endl;

    json empty_object_explicit = json::object();
    std::cout << "json::object();"              << std::endl;
    std::cout << empty_object_explicit          << std::endl;
    std::cout << empty_object_explicit.empty()  << std::endl << std::endl;

    json empty_array_explicit = json::array();
    std::cout << "json::array();"               << std::endl;
    std::cout << empty_array_explicit           << std::endl;
    std::cout << empty_array_explicit.empty()   << std::endl << std::endl;

    auto nothing_msg = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json{{}}.dump())));
    std::cout << "auto nothing_msg = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json{{}}.dump())));"                    << std::endl;
    std::cout << nothing_msg                    << std::endl;
    std::cout << nothing_msg.empty()            << std::endl << std::endl;

    auto nothing_msg_1 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json{{"abc","def"}}.dump())));
    std::cout << "auto nothing_msg_1 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json{{\"abc\",\"def\"}}.dump())));"                    << std::endl;
    std::cout << nothing_msg_1                    << std::endl;
    std::cout << nothing_msg_1.empty()            << std::endl << std::endl;

    auto nothing_msg_2 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json({}).dump())));
    std::cout << "auto nothing_msg_2 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json({}).dump())));"                    << std::endl;
    std::cout << nothing_msg_2                    << std::endl;
    std::cout << nothing_msg_2.empty()            << std::endl << std::endl;

    auto nothing_msg_3 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json::object().dump())));
    std::cout << "auto nothing_msg_3 = json::parse(boost::beast::buffers_to_string(boost::asio::buffer(json::object().dump())));"                    << std::endl;
    std::cout << nothing_msg_3                    << std::endl;
    std::cout << nothing_msg_3.empty()            << std::endl << std::endl;

    // auto nothing_msg_1 = json::parse(NULL);
    // std::cout << nothing_msg_1 << std::endl;
    // std::cout << nothing_msg_1.empty() << std::endl;

    // auto nothing_msg_2 = json::parse({[]});
    // std::cout << nothing_msg_2 << std::endl;
    // std::cout << nothing_msg_2.empty() << std::endl;
}

int main() {
    boost::asio::io_context ioc;
    tcp::resolver resolver(ioc);

    std::cout << "[Client] auto const results = resolver.resolve(\"localhost\", \"8080\");\n";
    auto const results = resolver.resolve("server", "8080");
    websocket_stream ws(ioc);
    
    bool connected = false;

    // Keep attempting to connect until successful
    while (!connected) {
        try {
            std::cout << "[Client] connect with localhost\n";
            auto ep = boost::asio::connect(ws.next_layer(), results);
            std::cout << "[Client] Handshake with localhost\n";
            // ws.handshake("localhost", "/");
            ws.handshake("server", "/");
            connected = true;
        } catch (const std::exception& e) {
            std::cout << "[Client]" << e.what() <<"\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "[Client] Writing Heartbeat Period Request\n";
    ws.write(boost::asio::buffer(json({}).dump()));
    std::cout << "[Client] Wrote Heartbeat Period Request\n";

    boost::beast::multi_buffer buffer;
    
    ws.read(buffer);
    std::cout << "[Client] Got Heartbeat Period Response\n";
    // print_different_json_formats();

    try {
        auto buffer_parsed = json::parse(boost::beast::buffers_to_string(buffer.data()));
        std::cout << buffer_parsed << std::endl;
        std::cout << buffer_parsed.empty() << std::endl;
        auto period = buffer_parsed["period"];
        for (;;) {
            auto period_in_seconds = std::chrono::seconds(period);

            std::cout << "[Client] Sleeping for "  << period << "\n";
            std::this_thread::sleep_for(period_in_seconds);
            std::cout << "[Client] Done Sleeping\n";

            std::cout << "[Client] Writing Heartbeat Request \n";
            ws.write(boost::asio::buffer(json{{"client_id", "client_number_1"}}.dump()));
            std::cout << "[Client] Wrote Heartbeat Request\n";

            ws.read(buffer);
            std::cout << "[Client] Got Heartbeat Response\n";
        }
    } catch (const std::exception& e) {
        std::cout << "[Client] loop" << e.what() <<"\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }


}
