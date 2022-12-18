#ifndef UTIL_COLOR_HSV_HPP
#define UTIL_COLOR_HSV_HPP

#include "color_def.hpp"

namespace util
{

template<>
class color<util::color_system::hsv> {
public:

    COLOR_COUT(hsv)
    COLOR_TO()
    COLOR_TO_SELF(hsv)
};

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::hsv>& t_col) {
    return t_out;
}

}

#endif
