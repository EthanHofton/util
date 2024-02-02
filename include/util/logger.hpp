#ifndef UTIL_LOGGER_HPP
#define UTIL_LOGGER_HPP

#include "algo.hpp"
#include "colormod.hpp"

#include <unistd.h>
#include <map>
#include <sstream>
#include <thread>

namespace util
{
    /**
    * @brief enum class to store the logger sink priority levels
    */
    enum class LOGGER_SINK_LEVEL
    {
        TRACE    = 0,
        DEBUG    = 1,
        INFO     = 2,
        WARNING  = 3,
        ERROR    = 4,
        CRITICAL = 5,
        NONE     = 6,
    };

    /**
     * @brief enum class to store the logger priority level
     * 
     */
    enum class LOGGER_LEVEL
    {
        TRACE    = 0,
        DEBUG    = 1,
        INFO     = 2,
        WARNING  = 3,
        ERROR    = 4,
        CRITICAL = 5,
    };

    /**
     * @brief LOGGER_LEVEL standard out
     * 
     * @param t_os the output stream to write
     * @param t_level the log level to output
     * @return std::ostream& 
     */
    inline std::ostream& operator<<(std::ostream& t_os, const LOGGER_LEVEL& t_level)
    {
        switch (t_level)
        {
            case LOGGER_LEVEL::TRACE:    return t_os << "TRACE";
            case LOGGER_LEVEL::DEBUG:    return t_os << "DEBUG";
            case LOGGER_LEVEL::INFO:     return t_os << "INFO";
            case LOGGER_LEVEL::WARNING:  return t_os << "WARNING";
            case LOGGER_LEVEL::ERROR:    return t_os << "ERROR";
            case LOGGER_LEVEL::CRITICAL: return t_os << "CRITICAL";
        }
    }

    /**
    * @brief LOGGER_SINK_LEVEL standard out
    *
    * @param t_os the output stream to write to
    * @param t_level the logger sink level to output
    *
    * @return std::ostream&
    */
    inline std::ostream& operator<<(std::ostream& t_os, const LOGGER_SINK_LEVEL& t_level)
    {
        switch (t_level)
        {
            case LOGGER_SINK_LEVEL::TRACE:    return t_os << "TRACE";
            case LOGGER_SINK_LEVEL::DEBUG:    return t_os << "DEBUG";
            case LOGGER_SINK_LEVEL::INFO:     return t_os << "INFO";
            case LOGGER_SINK_LEVEL::WARNING:  return t_os << "WARNING";
            case LOGGER_SINK_LEVEL::ERROR:    return t_os << "ERROR";
            case LOGGER_SINK_LEVEL::CRITICAL: return t_os << "CRITICAL";
            case LOGGER_SINK_LEVEL::NONE:     return t_os << "NONE";
        }
    }

    class logger;
    /**
     * @brief sink for a logger. a way for the message of the logger to be outputted
     * 
     * Default logger format: [%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v
     * Defualt logger level : INFO 
     * 
     */
    class logger_sink
    {
    public:

        /**
         * @brief Construct a new logger sink object
         * 
         */
        logger_sink() {}
        /**
         * @brief Destroy the logger sink object
         * 
         */
        virtual ~logger_sink() = 0;

        /**
         * @brief Set the Log Level
         * 
         * @param t_level the level to set the logger priority
         */
        void setLogLevel(const LOGGER_SINK_LEVEL& t_level) { m_level = t_level; }
        /**
         * @brief Get the Log Level as int
         * 
         * @return int 
         */
        int getLogLeveli() const { return (int)m_level; }
        /**
        * @brief get log level
        *
        * @return LOGGER_SINK_LEVEL
        */
        LOGGER_SINK_LEVEL getLogLevel() const { return m_level; }

        /**
         * @brief Set the output format
         * 
         * @param t_format the format to set
         */
        void setFormat(const std::string& t_format) { m_format = t_format; }
        /**
         * @brief Get the output format
         * 
         * @return std::string 
         */
        std::string getFormat() { return m_format; }

        /**
         * @brief output a message. to be defined by derrived classes
         * 
         * @param t_out the message to output
         */
        virtual void output(const std::string& t_out) const = 0;

        /**
         * @brief add special charicters to the output stream to define colors
         * 
         * @param t_level the level of the output to base the color off
         * @return std::string 
         */
        virtual std::string startOutputColorRange(const LOGGER_LEVEL&) const { return ""; }
        /**
         * @brief restore the old color for the output stream
         * 
         * @return std::string 
         */
        virtual std::string endOutputColorRange() const { return ""; }

        /**
         * @brief return a new copy of derivded class as the base class from the derivded class
         * 
         * @return logger_sink* 
         */
        virtual logger_sink* clone() const = 0;

        #define LOGGER_SINK_CLONE_IMPL(derived) logger_sink* clone() const override { return new derived(static_cast<const derived&>(*this)); }

    protected:
        /**
         * @brief the format to output the message in
         * 
         * default format: [%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v
         * @see logger
         * 
         */
        std::string m_format = "[%Y-%m-%d %H:%M:%S] [%n] [%^%l%$] %v";

    private:
        std::string m_loggerName;
        LOGGER_SINK_LEVEL m_level = LOGGER_SINK_LEVEL::INFO;
        friend logger;
    };

    /**
     * @brief Destroy the logger sink object
     * 
     */
    inline logger_sink::~logger_sink() {}

    /**
     * @brief logger class. takes sinks to output structed messages too.
     * 
     * the logger level heigherchy is as follows: TRACE->DEBUG->INFO->WARNING->ERROR->CRITICAL
     * the logger output format is as follows:
     * %v: the text to log
     * %t: thread id
     * %P: process id
     * %n: logger name
     * %l: logger level
     * %a: abbriviated weekday name
     * %A: full weekday name
     * %b: abbriviated month name
     * %B: full month name
     * %c: date and time representation (%a %b d %H:%M:%S %Y)
     * %C: Year in 2 digits
     * %Y: year in 4 digits
     * %D or %x: short for %m/%d/%C
     * %m: month 01-12
     * %d: day of the month 01-31
     * %H: hour in 24 hour format
     * %I: hout in 12 hour format
     * %M: minuets 00-59
     * %S: seconds 00-59
     * %p: AM/PM
     * %r: 24hr clock (%I:%M:S %p)
     * %R: 24 hour (%H:%M)
     * %T or %X: ISO 8601 format (%H:%M:%S)
     * %z: ISO 8601 offset from UTC in timezone ([+/-]HH:MM)
     * %E: second since Unix Epoch
     * %^: start color range
     * %$: end color range
     * %g: __FILE__
     * 
     */
    class logger 
    {
    public:

        /**
         * @brief Construct a new logger object
         * 
         */
        logger() {}
        /**
         * @brief Destroy the logger object and free all the sinks that were created
         * 
         */
        ~logger() { for (auto sink : m_sinks) delete sink.second; m_sinks.clear(); }

        /**
         * @brief logger copy constructor. creates copied of all the sinks
         * 
         */
        logger(logger const& t_logger)
        {
            for (auto& pair : t_logger.m_sinks)
            {
                m_sinks[pair.first] = pair.second->clone();
            }
        }

        /**
         * @brief logger move constructor. moves ownership of the pervious sinks to the new class
         * 
         */
        logger(logger&& t_logger)
        { m_sinks = std::move(t_logger.m_sinks); }

        /**
         * @brief add a new output sink
         * 
         * @tparam T the type of sink. must be derrived from logger_sink class
         * @tparam Args the arguments used to construct the object
         * @param t_index the name to give to the logger (must be unique)
         * @param t_args the arguments used to construct the sink
         */
        template<typename T, typename... Args>
        void addSink(const std::string& t_index, Args&&... t_args)
        { m_sinks[t_index] = new T(t_args...); m_sinks[t_index]->m_loggerName = t_index; }
        /**
         * @brief remove an output sink
         * 
         * @param t_index the name used to identify the sink
         */
        void removeSink(const std::string& t_index)
        { if (m_sinks.find(t_index) != m_sinks.end()) { m_sinks.erase(t_index); } }

        /**
         * @brief Set the sinks log level
         * 
         * @param t_index sinks identifier
         * @param t_level level to set
         */
        void setSinkLogLevel(const std::string& t_index, const LOGGER_SINK_LEVEL& t_level)
        { m_sinks[t_index]->setLogLevel(t_level); }
        /**
         * @brief Get a sinks log level
         * 
         * @param t_index the sinks identifier
         * @return LOGGER_LEVEL 
         */
        LOGGER_SINK_LEVEL getSinkLogLevel(const std::string& t_index)
        { return m_sinks[t_index]->getLogLevel(); }

        /**
         * @brief Set the sinks Format (see logger class breif)
         * 
         * @see logger
         * @param t_index the sinks identifier
         * @param t_format the format to set
         */
        void setSinkFormat(const std::string& t_index, const std::string& t_format) { m_sinks[t_index]->setFormat(t_format); }
        /**
         * @brief Get the Sink Format string
         * 
         * @param t_index the sinks identifier
         * @return std::string 
         */
        std::string getSinkFormat(const std::string& t_index) { return m_sinks[t_index]->getFormat(); }

        /**
         * @brief log a message to an output sink
         * 
         * @param t_msg the message to log
         * @param t_level the priority of the message
         * @param t_sink the identifier of the sink to log to
         */
        void log(const std::string& t_msg, const LOGGER_LEVEL& t_level, const std::string& t_sink)
        { if ((int)t_level >= m_sinks[t_sink]->getLogLeveli()) m_sinks[t_sink]->output(format(t_sink, t_msg, t_level)); }

        /**
         * @brief log a message to all output sinks
         * 
         * @param t_msg the message to log
         * @param t_level the priority of the message
         */
        void log(const std::string& t_msg, const LOGGER_LEVEL& t_level)
        { for (auto sink : m_sinks) log(t_msg, t_level, sink.first); }

        /**
         * @brief log a message at trace prioirty to all sinks
         * 
         * @param t_fmt the message to log
         */
        void logTrace(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::TRACE); }
        /**
         * @brief log a message at trace priority to all sinks
         * 
         * @tparam Args the types of arguemnts passed
         * @param t_fmt a format string
         * @param t_args arguemnts to pass to format string
         */
        template <typename... Args>
        void logTrace(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::TRACE); }

        /**
         * @brief log a message at trace prirorty to a spicific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the message to log
         */
        void logSinkTrace(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::TRACE, t_sink); }
        /**
         * @brief log a message at trace priority to a specific sink
         * 
         * @tparam Args all the types of arguemnts
         * @param t_sink the sink to log to
         * @param t_fmt a format string to print
         * @param t_args the arguemnts to pass to the format string
         */
        template <typename... Args>
        void logSinkTrace(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::TRACE, t_sink); }

        /**
         * @brief log a message at debug prioirty to all sinks
         * 
         * @param t_fmt the message to log
         */
        void logDebug(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::DEBUG); }
        /**
         * @brief log a message at debug priority to all sinks
         * 
         * @tparam Args the types of arguemnts passed
         * @param t_fmt a format string
         * @param t_args arguemnts to pass to format string
         */
        template <typename... Args>
        void logDebug(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::DEBUG); }

        /**
         * @brief log a message at debug priority to a specific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the message to log
         */
        void logSinkDebug(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::DEBUG, t_sink); }
        /**
         * @brief log a message at debug priority to a specific sink
         * 
         * @tparam Args the types of arguemnts passed
         * @param t_sink the sink to log to
         * @param t_fmt a format string to log
         * @param t_args the arguments to pass to the format string
         */
        template <typename... Args>
        void logSinkDebug(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::DEBUG, t_sink); }

        /**
         * @brief log a message at info priorirty to all sinks
         * 
         * @param t_fmt the message to log
         */
        void logInfo(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::INFO); }
        /**
         * @brief log a message at info priority to all sinks
         * 
         * @tparam Args all the types of arguments passed
         * @param t_fmt a format string to be logged
         * @param t_args the arguments passed to the format string
         */
        template <typename... Args>
        void logInfo(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::INFO); }

        /**
         * @brief log a message at info priority to a specific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the message to log
         */
        void logSinkInfo(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::INFO, t_sink); }
        /**
         * @brief log a message at info priority to a speicfic sink
         * 
         * @tparam Args all the types of arguments passed
         * @param t_sink the sink to log to
         * @param t_fmt a format string to be logged
         * @param t_args the arguemnts passed to the format string
         */
        template <typename... Args>
        void logSinkInfo(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::INFO, t_sink); }

        /**
         * @brief log a message at warning priotity to all sinks
         * 
         * @param t_fmt the message to log
         */
        void logWarning(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::WARNING); }
        /**
         * @brief log a message at warning priority to all sinks
         * 
         * @tparam Args all the types of arguments passed
         * @param t_fmt a format string to be logged
         * @param t_args the arguemtns passed to the format string
         */
        template <typename... Args>
        void logWarning(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::WARNING); }

        /**
         * @brief log a message at warning priority to a specific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the message to log
         */
        void logSinkWarning(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::WARNING, t_sink); }
        /**
         * @brief log a message at warning priority to a sprcific sink
         * 
         * @tparam Args all the types of the arguments passed
         * @param t_sink the sink to log to
         * @param t_fmt the format string to log
         * @param t_args the arguemnts to be passed to the format string
         */
        template <typename... Args>
        void logSinkWarning(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::WARNING, t_sink); }

        /**
         * @brief log a message at error priority to all sinks
         * 
         * @param t_fmt the message to log
         */
        void logError(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::ERROR); }
        /**
         * @brief log a message at error priority to all sinks
         * 
         * @tparam Args all the types of the arguments passed
         * @param t_fmt the format string to log
         * @param t_args the arguemnts to be passed to the format string
         */
        template <typename... Args>
        void logError(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::ERROR); }

        /**
         * @brief log a message at error priority to a specific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the message to log
         */
        void logSinkError(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::ERROR, t_sink); }
        /**
         * @brief log a message at error priority to a specific sink
         * 
         * @tparam Args the types of the arguments passed in
         * @param t_sink the sink to log to
         * @param t_fmt the format string to log
         * @param t_args the arugments to pass to the format string
         */
        template <typename... Args>
        void logSinkError(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::ERROR, t_sink); }

        /**
         * @brief log a message at critical priority to all sinks
         * 
         * @param t_fmt the message to be logged
         */
        void logCritical(const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::CRITICAL); }
        /**
         * @brief log a message at critical priority to all sinks
         * 
         * @tparam Args all the types of arguments passed in
         * @param t_fmt the format string to log
         * @param t_args the arguments to pass to the format string
         */
        template <typename... Args>
        void logCritical(const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::CRITICAL); }

        /**
         * @brief log a message at critical priority to a specific sink
         * 
         * @param t_sink the sink to log to
         * @param t_fmt the format string to log
         */
        void logSinkCritical(const std::string& t_sink, const std::string& t_fmt)
        { log(t_fmt, LOGGER_LEVEL::CRITICAL, t_sink); }
        /**
         * @brief log a message at critical priority to a specfic sink
         * 
         * @tparam Args all the types of the arguemtns packed in
         * @param t_sink the sink to log to
         * @param t_fmt the format string to log
         * @param t_args the arguemnts passed to the format string
         */
        template <typename... Args>
        void logSinkCritical(const std::string& t_sink, const std::string& t_fmt, Args&&... t_args)
        { log(fmt(t_fmt, t_args...), LOGGER_LEVEL::CRITICAL, t_sink); }

    private:

        std::string format(const std::string& t_sink, const std::string& t_msg, const LOGGER_LEVEL& t_level)
        {
            std::string result = m_sinks[t_sink]->getFormat();
            size_t pos = result.find("%");
            while (pos != std::string::npos)
            {
                if (pos + 1 > result.size())
                {
                    throw std::invalid_argument("no arg provided");
                }

                char command = result[pos+1];
                std::string replValue = "";
                switch(command)
                {
                    case 'v': replValue = t_msg; break;
                    case 't': { std::ostringstream ss; ss << std::this_thread::get_id(); replValue = ss.str(); } break;
                    case 'P': replValue = std::to_string(getpid()); break;
                    case 'n': replValue = t_sink; break;
                    case 'l': { std::stringstream ss; ss << t_level; replValue = ss.str(); } break;
                    case 'g': replValue = __FILE__; break;
                    case 'E': replValue = std::to_string(std::time(0)); break;
                    case 'a': case 'A': case 'b': case 'B': case 'c': case 'm': 
                    case 'H': case 'I': case 'M': case 'S': case 'p': case 'r': 
                    case 'T': case 'X': case 'z': case 'R': case 'd':
                        replValue = strfmttime(std::string("%") + command); break;
                    case 'C': replValue = strfmttime("%g"); break;
                    case 'Y': replValue = strfmttime("%G"); break;
                    case 'D': case 'x': replValue = strfmttime("%D"); break;
                    case '^': replValue = m_sinks[t_sink]->startOutputColorRange(t_level); break;
                    case '$': replValue = m_sinks[t_sink]->endOutputColorRange(); break;
                }

                result.replace(pos, 2, replValue);
                pos = result.find("%");
            }

            return result;
        }

        std::map<std::string, logger_sink*> m_sinks;
    };
}

#endif
