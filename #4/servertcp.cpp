#include <iostream>
#include <boost/asio.hpp>
#include <vector>
#include <string>

using boost::asio::ip::tcp;

// Циклический буфер
class CircularBuffer {
public:
    CircularBuffer(size_t capacity) 
        : buffer_(capacity), head_(0), tail_(0), size_(0) {}

    void push_back(char value) {
        if (size_ < buffer_.size()) {
            buffer_[tail_] = value;
            tail_ = (tail_ + 1) % buffer_.size();
            ++size_;
        } else {
            std::cerr << "Buffer overflow" << std::endl;
        }
    }

    char pop_front() {
        if (size_ > 0) {
            char value = buffer_[head_];
            head_ = (head_ + 1) % buffer_.size();
            --size_;
            return value;
        } else {
            throw std::out_of_range("Buffer underflow");
        }
    }

    size_t size() const {
        return size_;
    }

private:
    std::vector<char> buffer_;
    size_t head_;
    size_t tail_;
    size_t size_;
};

int main() {
    try {
        boost::asio::io_context io_context;

        // TCP-сервер прослушивает порт 12345
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 12345));
        std::cout << "Server started, waiting for connection..." << std::endl;

        //Принимаем соединение от клиента
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected." << std::endl;

        CircularBuffer circularBuffer(1024); // Размер буфера 1024 байт

        //  Читаем данные от клиента
        while (true) {
            std::vector<char> data(1024);
            boost::system::error_code error;

            size_t length = socket.read_some(boost::asio::buffer(data), error);

            if (error == boost::asio::error::eof) {
                std::cout << "Connection closed by client." << std::endl;
                break;
            } else if (error) {
                std::cerr << "Error reading from socket: " << error.message() << std::endl;
                break;
            }

            // Записываем данные в циклический буфер
            for (size_t i = 0; i < length; ++i) {
                circularBuffer.push_back(data[i]);
            }

            std::cout << "Received data from client: ";
            for (size_t i = 0; i < length; ++i) {
                std::cout << data[i];
            }
            std::cout << std::endl;

            // Отправляем подтверждение клиенту
            std::string response = "Message received";
            boost::asio::write(socket, boost::asio::buffer(response));

            std::cout << "Sent acknowledgment to client." << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}