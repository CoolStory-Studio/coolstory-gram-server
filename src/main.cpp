#include "CSGServer/main.hpp"

int main(int argc, char** argv) {
    logger::init(logger::debug, logger::cout);

    if (argc == 1) {
        std::cout <<
                  "Usage: coolstory_gram_server <address> <port> <threads>\n" <<
                  "Example:\n" <<
                  "    coolstory_gram_server 0.0.0.0 8080 1\n";
        return EXIT_SUCCESS;
    }
    else if (argc != 4) {
        std::cerr <<
                  "Usage: coolstory_gram_server <address> <port> <threads>\n" <<
                  "Example:\n" <<
                  "    coolstory_gram_server 0.0.0.0 8080 1\n";
        return EXIT_FAILURE;
    }
    auto const address = net::ip::make_address(argv[1]);
    auto const port = static_cast<unsigned short>(std::atoi(argv[2]));
    auto const threads = std::max<int>(1, std::atoi(argv[3]));
    run(address, port, threads);
    return EXIT_SUCCESS;
}
