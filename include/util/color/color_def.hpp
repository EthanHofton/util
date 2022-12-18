#ifndef UTIL_COLOR_COLOR_DEF
#define UTIL_COLOR_COLOR_DEF

#include <iostream>
#include <sstream>

namespace util
{

inline bool is_hex_str(const char t_hex[6])
{ for (int i = 0; i < 6; i++) if (!isxdigit(t_hex[i])) return false; return true; }

enum class color_system
{
    rgb,
    rgb_i,
    rgba,
    rgba_i,
    hex,
    hsv,
};

#define COLOR_COUT(type) friend std::ostream& operator<<(std::ostream&, const color<color_system::type>& t_col);
#define COLOR_TO() template<color_system T> \
color<T> to() const { throw std::invalid_argument("this conversion is not implamented yet"); }
#define COLOR_TO_SELF(type) template<> \
color<util::color_system::type> to() const { return *(this); }

template<color_system T> struct color;

typedef color<color_system::rgb> color_rgb;
typedef color<color_system::rgb_i> color_rgbi;
typedef color<color_system::rgba> color_rgba;
typedef color<color_system::rgba_i> color_rgbai;
typedef color<color_system::hex> color_hex;
typedef color<color_system::hsv> color_hsv;

#define COLOR_OPERATORS_FOR(type, arg_type) type& operator+=(const arg_type&); \
    type& operator-=(const arg_type&); \
    type& operator/=(const arg_type&); \
    type& operator*=(const arg_type&); \
    friend type operator+(const type&, const arg_type&); \
    friend type operator-(const type&, const arg_type&); \
    friend type operator/(const type&, const arg_type&); \
    friend type operator*(const type&, const arg_type&); \
    friend bool operator==(const type&, const arg_type&); \
    friend bool operator!=(const type&, const arg_type&); \
    friend bool operator<=(const type&, const arg_type&); \
    friend bool operator>=(const type&, const arg_type&); \
    friend bool operator<(const type&, const arg_type&); \
    friend bool operator>(const type&, const arg_type&);
    

#define COLOR_BASE_OPERATORS(type) type& operator++(); \
    type operator++(int); \
    type& operator--(); \
    type operator--(int); \
    COLOR_OPERATORS_FOR(type, type)

}

#endif
