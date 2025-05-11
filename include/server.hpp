#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <functional>

#include "handlers/interface.hpp"
#include "middleware/logger.hpp"

struct ServerConfig
{
    uint16_t port = 8080;
    uint16_t threads = 2;
    std::string logLevel = "info";
    bool cors = true;
    std::string corsOrigin = "*";
};

using App = crow::App<crow::CORSHandler, LoggerMiddleware>;

class Server
{
public:
    explicit Server(const ServerConfig &config = ServerConfig());
    ~Server() = default;

    void setup();
    void start();

    void addHandler(std::shared_ptr<IHandler> handler);

private:
    ServerConfig config_;
    std::unique_ptr<App> app_;
    std::vector<std::shared_ptr<IHandler>> handlers_;

    bool is_running_ = false;
};
