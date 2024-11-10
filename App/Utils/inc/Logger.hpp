#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string_view>
#include "fmt/core.h"
#include "fmt/color.h"
#include "fmt/ranges.h"

enum class Type
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger
{
public:
    Logger()
    {}

    static auto LOG(std::string_view msg, const Type type) -> void
    {
        switch (type)
        {
            case Type::WARNING:
                fmt::print(fg(fmt::color::yellow), "[WARNING] | {}\n", msg.data());
                break;
            case Type::INFO:
                fmt::print(fg(fmt::color::ghost_white), "[INFO] | {}\n", msg.data());
                break;
            case Type::ERROR:
                fmt::print(fg(fmt::color::red), "[ERROR] | {}\n", msg.data());
                break;
            case Type::DEBUG:
                fmt::print(fg(fmt::color::green), "[DEBUG] | {}\n", msg.data());
            default:
                break;
        }
    }

    ~Logger() = default;
};

#endif