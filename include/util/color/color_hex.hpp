#ifndef UTIL_COLOR_HEX_HPP
#define UTIL_COLOR_HEX_HPP

#include "color_def.hpp"

namespace util
{

template<>
struct color<util::color_system::hex> {
    
public:

    color(const char* t_hex) {
        if (std::strlen(t_hex) != 6) {
            throw std::invalid_argument("input is not a hex string");
        }

        if (!is_hex_str(t_hex)) {
            throw std::invalid_argument("input is not a hex string");
        }

        memcpy((void*)m_hex, (void*)t_hex, 6);
    }

    color(const std::string& t_hex) {
        if (t_hex.length() != 6) {
            throw std::invalid_argument("input is not a hex string");
        }

        if (!is_hex_str(t_hex.c_str())) {
            throw std::invalid_argument("input is not a hex string");
        }

        memcpy((void*)m_hex, (void*)t_hex.c_str(), 6);
    }

    COLOR_COUT(hex)
    COLOR_TO()
    template <> color<util ::color_system ::hex> to() const { return *(this); }

  private:

    char m_hex[6];
};

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::hex>& t_col) {

    t_out << "#" << std::string(t_col.m_hex, 6);

    return t_out;
}

}

#endif
