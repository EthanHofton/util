#ifndef UTIL_COLOR_RGBA_HPP
#define UTIL_COLOR_RGBA_HPP

#include "color_def.hpp" 

namespace util
{
template<>
struct color<util::color_system::rgba> {

public:

    color(const float& t_r, const float& t_g, const float& t_b, const float& t_a) : m_r(t_r), m_g(t_g), m_b(t_b), m_a(t_a) {}

    COLOR_COUT(rgba)
    COLOR_TO()
    COLOR_TO_SELF(rgba)

    float& r() { return m_r; }
    float& g() { return m_g; }
    float& b() { return m_b; }
    float& a() { return m_a; }

private:

    float m_r,m_g,m_b,m_a;
};

template<>
struct color<util::color_system::rgba_i> {

public:

    color(const int& t_r, const int& t_g, const int& t_b, const int& t_a) : m_r(t_r), m_g(t_g), m_b(t_b), m_a(t_a) {}

    COLOR_COUT(rgba_i)
    COLOR_TO()
    COLOR_TO_SELF(rgba_i)

    int& r() { return m_r; }
    int& g() { return m_g; }
    int& b() { return m_b; }
    int& a() { return m_a; }

private:

    int m_r,m_g,m_b,m_a;
};

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgba>& t_col) {

    t_out << "(r: " << t_col.m_r << "%, g: " << t_col.m_g << "%, b: " << t_col.m_g << "%, a: " << t_col.m_a << "%)";

    return t_out;
}

inline std::ostream& operator<<(std::ostream& t_out, const color<color_system::rgba_i>& t_col) {

    t_out << "(r: " << t_col.m_r << ", g: " << t_col.m_g << ", b: " << t_col.m_g << ", a: " << t_col.m_a << ")";

    return t_out;
}

}

#endif
