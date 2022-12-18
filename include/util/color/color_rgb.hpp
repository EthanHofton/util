#ifndef UTIL_COLOR_RGB_HPP
#define UTIL_COLOR_RGB_HPP

#include "color_def.hpp"

namespace util
{

template<>
struct color<util::color_system::rgb> {

public:

    // * constructor conversion *
    color(const float& t_r, const float& t_g, const float& t_b) : m_r(t_r), m_g(t_g), m_b(t_b) {}
    color(const color_rgbi&);
    color(const color_rgba&);
    color(const color_rgbai&);
    color(const color_hex&);
    color(const color_hsv&);

    // * operators *
    COLOR_COUT(rgb)
    float operator[](const int&) const;
    COLOR_BASE_OPERATORS(color_rgb)
    COLOR_OPERATORS_FOR(color_rgb, color_rgbi)
    COLOR_OPERATORS_FOR(color_rgb, color_rgba)
    COLOR_OPERATORS_FOR(color_rgb, color_rgbai)
    COLOR_OPERATORS_FOR(color_rgb, color_hex)
    COLOR_OPERATORS_FOR(color_rgb, color_hsv)
    // * operators *

    // * methods *
    COLOR_TO()
    COLOR_TO_SELF(rgb)

    float r() const { return m_r; }
    float g() const { return m_g; }
    float b() const { return m_b; }

private:

    float m_r,m_g,m_b;
};

template<>
struct color<util::color_system::rgb_i> {

public:

    // * constructor conversion *
    color(const int& t_r, const int& t_g, const int& t_b) : m_r(t_r), m_g(t_g), m_b(t_b) {}
    color(const color_rgb&);
    color(const color_rgba&);
    color(const color_rgbai&);
    color(const color_hex&);
    color(const color_hsv&);

    // * operators *
    COLOR_COUT(rgb_i)
    int operator[](const int&) const;
    COLOR_BASE_OPERATORS(color_rgbi)
    COLOR_OPERATORS_FOR(color_rgbi, color_rgb)
    COLOR_OPERATORS_FOR(color_rgbi, color_rgba)
    COLOR_OPERATORS_FOR(color_rgbi, color_rgbai)
    COLOR_OPERATORS_FOR(color_rgbi, color_hex)
    COLOR_OPERATORS_FOR(color_rgbi, color_hsv)
    // * operators *

    // * methods *
    COLOR_TO()
    COLOR_TO_SELF(rgb)

    int r() const { return m_r; }
    int g() const { return m_g; }
    int b() const { return m_b; }

private:

    float m_r,m_g,m_b;
};

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgb>& t_col) {

    t_out << "(r: " << t_col.m_r << "%, g: " << t_col.m_g << "%, b: " << t_col.m_b << "%)";

    return t_out;
}

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgb_i>& t_col) {

    t_out << "(r: " << t_col.m_r << ", g: " << t_col.m_g << ", b: " << t_col.m_b << ")";

    return t_out;
}

}

#endif
