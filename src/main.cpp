#include <iostream>
#include <memory>
#include <stdexcept>
#include "server.hpp"

int main()
{
    try
    {
        ServerConfig config;
        config.port = 8080;
        config.threads = 2;
        config.logLevel = "info";
        config.cors = true;
        config.corsOrigin = "*";

        auto server = std::make_unique<Server>(config);
        server->start();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}