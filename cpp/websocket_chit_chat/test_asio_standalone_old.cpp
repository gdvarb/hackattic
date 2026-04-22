#define ASIO_ENABLE_OLD_SERVICES
#include <asio.hpp>
#include <iostream>

int main() {
    asio::io_service service;
    std::cout << "asio::io_service exists" << std::endl;
    return 0;
}
