#ifndef UTIL_LOGGER_FILE_SINK_HPP
#define UTIL_LOGGER_FILE_SINK_HPP

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include "logger.hpp"

namespace util
{
    /**
     * @brief logger output sink to a file
     * 
     */
    class logger_file_sink : public logger_sink
    {
    public:

        /**
         * @brief Construct a new logger file sink object
         * 
         * @param t_filename the filename of the output file
         */
        logger_file_sink(const std::string& t_filename) : m_filename(t_filename) {}
        /**
         * @brief Construct a new logger file sink object
         * 
         * @param t_filename the filename of the ouput file
         * @param t_format the format output of the sink
         */
        logger_file_sink(const std::string& t_filename, const std::string& t_format) : logger_file_sink(t_filename)
        { m_format = t_format; }
        /**
         * @brief Destroy the logger file sink object
         * 
         */
        ~logger_file_sink() override {}

        /**
         * @brief output the message to the log file
         * 
         * @param t_out the message to output
         */
        void output(const std::string& t_out) const override
        {
            std::ofstream file(m_filename, std::ios::out | std::ios::app);
            file << t_out << std::endl;
            file.close();
        }

        /**
         * @brief Construct a new logger sink copy
         * 
         */
        LOGGER_SINK_CLONE_IMPL(logger_file_sink)

    private:

        std::string m_filename;
    };
}

#endif