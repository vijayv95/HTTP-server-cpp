#pragma once

#include <crow.h>
#include <iostream>

struct LoggerMiddleware
{
    struct context
    {
    };

    void before_handle(crow::request &req, crow::response &res, context &ctx)
    {
        CROW_LOG_INFO << req.url << "\n";
    }

    void after_handle(crow::request &req, crow::response &res, context &ctx)
    {
        CROW_LOG_INFO << req.url << " → " << res.code << "\n";
    }
};