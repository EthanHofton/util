#ifndef UTIL_LOGGER_CONSOLE_SINK_HPP
#define UTIL_LOGGER_CONSOLE_SINK_HPP

#include "logger.hpp"

namespace util
{
    /**
     * @brief logger output sink to console
     * 
     */
    class logger_console_sink : public logger_sink
    {
    public:

        /**
         * @brief Construct a new logger console sink object
         * 
         */
        logger_console_sink() {}
        /**
         * @brief Construct a new logger console sink object
         * 
         * @param t_format the output format of the sink
         */
        logger_console_sink(const std::string& t_format) { m_format = t_format; }
        /**
         * @brief Construct a new logger console sink object (string move)
         * 
         * @param t_format the output format of the sinks
         */
        logger_console_sink(const std::string&& t_format) { m_format = std::move(t_format); }
        /**
         * @brief Destroy the logger console sink object
         * 
         */
        ~logger_console_sink() override {}

        /**
         * @brief output a string to standard output
         * 
         * @param t_out the message to output
         */
        void output(const std::string& t_out) const override
        {
            std::cout << t_out << std::endl;
        }

        /**
         * @brief gets the color code to be outputted to the console
         * 
         * @param t_level the priority level of the message to log
         * @return std::string 
         */
        std::string startOutputColorRange(const LOGGER_LEVEL& t_level) const override
        {
            std::stringstream ss;

            switch (t_level)
            {
                case LOGGER_LEVEL::TRACE:    ss << CONSOLE_COLOR_CODE::FG_WHITE;                               break;
                case LOGGER_LEVEL::DEBUG:    ss << CONSOLE_COLOR_CODE::FG_LIGHT_MAGENTA;                       break;
                case LOGGER_LEVEL::INFO:     ss << CONSOLE_COLOR_CODE::FG_BLUE;                                break;
                case LOGGER_LEVEL::WARNING:  ss << CONSOLE_COLOR_CODE::FG_YELLOW;                              break;
                case LOGGER_LEVEL::ERROR:    ss << CONSOLE_COLOR_CODE::FG_RED;                                 break;
                case LOGGER_LEVEL::CRITICAL: ss << CONSOLE_COLOR_CODE::BG_RED << CONSOLE_COLOR_CODE::FG_BLACK; break;
            }

            return ss.str();
        }

        /**
         * @brief restores the old color code of the console
         * 
         * @return std::string 
         */
        std::string endOutputColorRange() const override
        {
            std::stringstream ss;
            ss << CONSOLE_COLOR_CODE::BG_DEFAULT << CONSOLE_COLOR_CODE::FG_DEFAULT;
            return ss.str();
        }

        /**
         * @brief create a new logger sink copy object
         * 
         */
        LOGGER_SINK_CLONE_IMPL(logger_console_sink)
    };
}

#endif