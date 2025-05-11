#pragma once

#include <crow.h>
#include <crow/middlewares/cors.h>
#include <string>

#include "middleware/logger.hpp"

using App = crow::App<crow::CORSHandler, LoggerMiddleware>;

class IHandler
{
public:
    virtual ~IHandler() = default;
    virtual void registerRoutes(App &app) = 0;
    virtual std::string getName() const = 0;
};