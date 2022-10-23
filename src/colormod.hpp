#ifndef UTIL_COLORMOD_HPP
#define UTIL_COLORMOD_HPP

#include <iostream>

namespace util
{
    /**
     * @brief enum class to store the color codes that change the color for the output console
     * 
     */
    enum class CONSOLE_COLOR_CODE
    {
        FG_DEFAULT       = 39,
        FG_BLACK         = 30,
        FG_RED           = 31,
        FG_GREEN         = 32,
        FG_YELLOW        = 33,
        FG_BLUE          = 34,
        FG_MAGENTA       = 35,
        FG_CYAN          = 36,
        FG_LIGHT_GRAY    = 37,
        FG_DARK_GRAY     = 90,
        FG_LIGHT_RED     = 91,
        FG_LIGHT_GREEN   = 92,
        FG_LIGHT_YELLOW  = 93,
        FG_LIGHT_BLUE    = 94,
        FG_LIGHT_MAGENTA = 95,
        FG_LIGHT_CYAN    = 96,
        FG_WHITE         = 97,
        BG_RED           = 41,
        BG_GREEN         = 42,
        BG_BLUE          = 44,
        BG_DEFAULT       = 49
    };

    /**
     * @brief color mod standard out operator
     * 
     * @param t_os the output stream
     * @param t_mod the color mod code
     * @return std::ostream& 
     */
    std::ostream& operator<<(std::ostream& t_os, const CONSOLE_COLOR_CODE& t_mod)
    {
        return t_os << "\033[" << ((int)t_mod) << "m";
    }
}

#endif