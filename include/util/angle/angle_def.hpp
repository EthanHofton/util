#ifndef UTIL_ANGLE_ANGLE_DEF_HPP
#define UTIL_ANGLE_ANGLE_DEF_HPP

#include <iostream>

namespace util {

enum class angle_unit {
    degree,
    radian,
};

#define ANGLE_COUT(type) friend std::ostream& operator<<(std::ostream&, const angle<util::angle_unit::type>&);
#define ANGLE_TO() template<angle_unit T> \
    util::angle<T> to() const { throw std::invalid_argument("conversion implimented yet"); }
#define ANGLE_TO_SELF(type) template<> \
    util::angle<util::angle_unit::type> to() const { return *(this); }

template<angle_unit _unit> class angle; 

typedef angle<angle_unit::degree> angle_d;
typedef angle<angle_unit::radian> angle_r;

}

#endif
