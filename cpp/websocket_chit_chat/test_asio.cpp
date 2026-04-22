#include <boost/asio.hpp>
#include <iostream>

int main() {
    boost::asio::io_service service;
    std::cout << "io_service exists" << std::endl;
    return 0;
}
