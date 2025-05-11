
#include "server.hpp"
#include "handlers/users.hpp"

Server::Server(const ServerConfig &config) : config_(config)
{
    // Set up logging level before creating the app
    if (config_.logLevel == "debug")
    {
        crow::logger::setLogLevel(crow::LogLevel::Debug);
    }
    else if (config_.logLevel == "info")
    {
        crow::logger::setLogLevel(crow::LogLevel::Info);
    }
    else if (config_.logLevel == "warning")
    {
        crow::logger::setLogLevel(crow::LogLevel::Warning);
    }
    else if (config_.logLevel == "error")
    {
        crow::logger::setLogLevel(crow::LogLevel::Error);
    }
    else if (config_.logLevel == "critical")
    {
        crow::logger::setLogLevel(crow::LogLevel::Critical);
    }

    // Create app with middlewares
    app_ = std::make_unique<App>();

    // Configure CORS middleware
    auto &cors = app_->get_middleware<crow::CORSHandler>();
    cors
        .global()
        .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method, "OPTIONS"_method)
        .headers("Content-Type", "Authorization")
        .origin(config_.corsOrigin)
        .prefix("/api")
        .max_age(3600);
}

void Server::setup()
{
    this->addHandler(std::make_shared<UserHandler>("/api/users"));
}

void Server::start()
{
    this->setup();
    std::cout << "Server started on port " << config_.port << std::endl;
    app_->port(config_.port).multithreaded().run();
}

void Server::addHandler(std::shared_ptr<IHandler> handler)
{
    handlers_.push_back(handler);
    handler->registerRoutes(*this->app_);
}