#include <boost/asio.hpp>
#include <iostream>

int main() {
#ifdef BOOST_ASIO_NO_DEPRECATED
    std::cout << "BOOST_ASIO_NO_DEPRECATED is defined" << std::endl;
#else
    std::cout << "BOOST_ASIO_NO_DEPRECATED is NOT defined" << std::endl;
#endif
    return 0;
}
