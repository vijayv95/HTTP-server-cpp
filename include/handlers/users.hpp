#pragma once

#include <crow.h>
#include <unordered_map>
#include <mutex>
#include <string>
#include <vector>
#include <stdexcept>

#include "base.hpp"
#include "models/users.hpp"

class UserHandler : public BaseHandler
{
public:
    UserHandler(const std::string &basePath);
    void registerRoutes(App &app) override;

private:
    crow::response list(const crow::request &req);
    crow::response get(int id);
    crow::response create(const crow::request &req);
    crow::response update(int id, const crow::request &req);
    crow::response remove(int id);

    // Mock database
    std::unordered_map<int, User> users_;
    int lastID_;
    std::mutex mutex_;
};