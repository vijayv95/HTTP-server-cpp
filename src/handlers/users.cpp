#include "handlers/base.hpp"
#include "handlers/users.hpp"

UserHandler::UserHandler(const std::string &basePath) : BaseHandler(basePath)
{
    this->name_ = "UserHandler";
    this->lastID_ = 0;

    // Mock database
    this->lastID_ += 1;
    this->users_[this->lastID_] = User{1, "asad", "l0h2S@example.com"};
    this->lastID_ += 1;
    this->users_[this->lastID_] = User{2, "asad2", "l0h2S@example.com"};
}

crow::response UserHandler::list(const crow::request &req)
{
    auto q = req.url_params.get("q") == nullptr ? "" : req.url_params.get("q");
    int page = 1;
    int limit = 10;

    auto pageParam = req.url_params.get("page") ?: "1";
    try
    {
        page = std::max(1, std::stoi(pageParam));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    auto limitParam = req.url_params.get("limit") ?: "10";
    try
    {
        page = std::max(1, std::stoi(pageParam));
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    crow::json::wvalue response;
    crow::json::wvalue::list users;
    for (auto &user : this->users_)
    {
        crow::json::wvalue userJson;
        userJson["id"] = user.second.id;
        userJson["username"] = user.second.username;
        userJson["email"] = user.second.email;
        users.push_back(std::move(userJson));
    }
    response["users"] = std::move(users);

    return crow::response(crow::OK, response);
}

crow::response UserHandler::get(int id)
{
    if (this->users_.find(id) == this->users_.end())
    {
        return this->not_found("User not found");
    }

    User user = this->users_[id];

    crow::json::wvalue response;
    response["id"] = user.id;
    response["username"] = user.username;
    response["email"] = user.email;

    return crow::response(crow::OK, response);
}

crow::response UserHandler::create(const crow::request &req)
{
    crow::json::rvalue json = crow::json::load(req.body);
    std::string username = json["username"].s();
    std::string email = json["email"].s();

    this->mutex_.lock();

    this->lastID_ += 1;
    User user = User{this->lastID_, username, email};
    this->users_[this->lastID_] = user;

    this->mutex_.unlock();

    crow::json::wvalue response;
    response["id"] = user.id;
    response["username"] = user.username;
    response["email"] = user.email;

    return crow::response(crow::CREATED, response);
}

crow::response UserHandler::update(int id, const crow::request &req)
{
    if (this->users_.find(id) == this->users_.end())
    {
        return this->not_found("User not found");
    }

    crow::json::rvalue json = crow::json::load(req.body);
    std::string username = json["username"].s();
    std::string email = json["email"].s();

    this->mutex_.lock();

    User user = this->users_[id];
    user.username = username;
    user.email = email;
    this->users_[id] = user;

    this->mutex_.unlock();

    crow::json::wvalue response;
    response["id"] = user.id;
    response["username"] = user.username;
    response["email"] = user.email;

    return crow::response(crow::OK, response);
}

crow::response UserHandler::remove(int id)
{
    if (this->users_.find(id) == this->users_.end())
    {
        return this->not_found("User not found");
    }

    this->mutex_.lock();
    this->users_.erase(id);
    this->mutex_.unlock();

    crow::json::wvalue response;
    response["success"] = true;

    return crow::response(crow::OK, response);
}

void UserHandler::registerRoutes(App &app)
{
    app.route_dynamic(this->basePath_)
        .methods(crow::HTTPMethod::GET)(
            [this](const crow::request &req)
            {
                return this->list(req);
            });

    app.route_dynamic(this->basePath_ + "/<int>")
        .methods(crow::HTTPMethod::GET)(
            [this](const crow::request &req, int id)
            {
                return this->get(id);
            });

    app.route_dynamic(this->basePath_)
        .methods(crow::HTTPMethod::POST)(
            [this](const crow::request &req)
            {
                return this->create(req);
            });

    app.route_dynamic(this->basePath_ + "/<int>")
        .methods(crow::HTTPMethod::PUT)(
            [this](const crow::request &req, int id)
            {
                return this->update(id, req);
            });

    app.route_dynamic(this->basePath_ + "/<int>")
        .methods(crow::HTTPMethod::DELETE)(
            [this](const crow::request &req, int id)
            {
                return this->remove(id);
            });
}