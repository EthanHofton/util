#ifndef UTIL_SERVER_HPP
#define UTIL_SERVER_HPP

#include <cstdlib>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <logger.hpp>
#include <logger_console_sink.hpp>
#include <exception>
#include <future>
#include <vector>
#include <map>
#include <sys/time.h>
#include <arpa/inet.h>
#include <algo.hpp>

namespace util {

enum class SERVER_METHOD {
    TCP,
    UDP,
};

class logger_server_console_sink : public util::logger_sink {
public:

    static void str_overwrite_stdout() {
        printf("\r%s", "admin@server> ");
        fflush(stdout);
    }

    logger_server_console_sink() {
        m_format = "\r[%^%l%$] %v";
    }

    ~logger_server_console_sink() override {}

    void output(const std::string& t_out) const override {
        printf("\r%s\n", t_out.c_str());
        str_overwrite_stdout();
    }

    
    std::string startOutputColorRange(const util::LOGGER_LEVEL& t_level) const override
    {
        std::stringstream ss;

        switch (t_level)
        {
            case util::LOGGER_LEVEL::TRACE:    ss << util::CONSOLE_COLOR_CODE::FG_WHITE;                                       break;
            case util::LOGGER_LEVEL::DEBUG:    ss << util::CONSOLE_COLOR_CODE::FG_LIGHT_MAGENTA;                               break;
            case util::LOGGER_LEVEL::INFO:     ss << util::CONSOLE_COLOR_CODE::FG_BLUE;                                        break;
            case util::LOGGER_LEVEL::WARNING:  ss << util::CONSOLE_COLOR_CODE::FG_YELLOW;                                      break;
            case util::LOGGER_LEVEL::ERROR:    ss << util::CONSOLE_COLOR_CODE::FG_RED;                                         break;
            case util::LOGGER_LEVEL::CRITICAL: ss << util::CONSOLE_COLOR_CODE::BG_RED << util::CONSOLE_COLOR_CODE::FG_DEFAULT; break;
        }

        return ss.str();
    }

    std::string endOutputColorRange() const override
    {
        std::stringstream ss;
        ss << util::CONSOLE_COLOR_CODE::BG_DEFAULT << util::CONSOLE_COLOR_CODE::FG_DEFAULT;
        return ss.str();
    }

    LOGGER_SINK_CLONE_IMPL(logger_server_console_sink)
};

}

#endif
