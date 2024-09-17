#include <iostream>
#include <boost/asio.hpp>
#include <string>
#include <chrono>
#include <thread>

using boost::asio::ip::tcp;

int main() {
    try {
        boost::asio::io_context io_context;

        // Устанавливаем соединение с сервером на localhost:12345
        tcp::socket socket(io_context);
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");
        boost::asio::connect(socket, endpoints);

        std::cout << "Connected to the server." << std::endl;

        // Отправляем несколько сообщений на сервер
        std::vector<std::string> messages = {
            "Hello from client!",
            "This is message 2",
            "This is message 3"
        };

        for (const auto& message : messages) {
            // Отправляем сообщение
            boost::asio::write(socket, boost::asio::buffer(message));
            std::cout << "Sent: " << message << std::endl;

            // Читаем ответ от сервера
            char response[1024] = {0}; // Буфер для ответа
            size_t length = socket.read_some(boost::asio::buffer(response));
            std::cout << "Received acknowledgment from server: " << std::string(response, length) << std::endl;

            // Пауза перед отправкой следующего сообщения
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Закрываем соединение
        socket.close();
        std::cout << "Connection closed." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}