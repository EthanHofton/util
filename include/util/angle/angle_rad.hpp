#ifndef UTIL_ANGLE_ANGLE_RAD_HPP
#define UTIL_ANGLE_ANGLE_RAD_HPP

#include "angle_def.hpp"
#include "util/math.hpp"
#include <cmath>

namespace util {

template<>
class angle<angle_unit::radian> {
public:

    // * constructors *
    angle(const double& t_angle) : m_angle(t_angle) {}
    angle(const angle_r& t_angle) : m_angle(t_angle.m_angle) {}
    angle(const angle_d&);

    // * getters *
    double get() const { return m_angle; }

    // * operators *
    ANGLE_COUT(radian)

    angle_r operator+() const;
    angle_r operator-() const;
    angle_r& operator++();
    angle_r operator++(int);
    angle_r& operator--();
    angle_r operator--(int);
    angle_r& operator+=(const double&);
    angle_r& operator+=(const angle_r&);
    angle_r& operator+=(const angle_d&);
    angle_r& operator-=(const double&);
    angle_r& operator-=(const angle_r&);
    angle_r& operator-=(const angle_d&);
    angle_r& operator*=(const double&);
    angle_r& operator*=(const angle_r&);
    angle_r& operator*=(const angle_d&);
    angle_r& operator/=(const double&);
    angle_r& operator/=(const angle_r&);
    angle_r& operator/=(const angle_d&);
    friend angle_r operator+(const angle_r&, const angle_r&);
    friend angle_r operator+(const angle_r&, const double&);
    friend angle_r operator+(const angle_r&, const angle_d&);
    friend angle_r operator-(const angle_r&, const angle_r&);
    friend angle_r operator-(const angle_r&, const double&);
    friend angle_r operator-(const angle_r&, const angle_d&);
    friend angle_r operator*(const angle_r&, const angle_r&);
    friend angle_r operator*(const angle_r&, const double&);
    friend angle_r operator*(const angle_r&, const angle_d&);
    friend angle_r operator/(const angle_r&, const angle_r&);
    friend angle_r operator/(const angle_r&, const double&);
    friend angle_r operator/(const angle_r&, const angle_d&);

    friend bool operator==(const angle_r&, const angle_r&);
    friend bool operator==(const angle_r&, const double&);
    friend bool operator==(const angle_r&, const angle_d&);
    friend bool operator!=(const angle_r&, const angle_r&);
    friend bool operator!=(const angle_r&, const double&);
    friend bool operator!=(const angle_r&, const angle_d&);
    friend bool operator<=(const angle_r&, const angle_r&);
    friend bool operator<=(const angle_r&, const double&);
    friend bool operator<=(const angle_r&, const angle_d&);
    friend bool operator>=(const angle_r&, const angle_r&);
    friend bool operator>=(const angle_r&, const double&);
    friend bool operator>=(const angle_r&, const angle_d&);
    friend bool operator>(const angle_r&, const angle_r&);
    friend bool operator>(const angle_r&, const double&);
    friend bool operator>(const angle_r&, const angle_d&);
    friend bool operator<(const angle_r&, const angle_r&);
    friend bool operator<(const angle_r&, const double&);
    friend bool operator<(const angle_r&, const angle_d&);

    // * methods
    ANGLE_TO()
    ANGLE_TO_SELF(radian)

    friend double sin(const angle_r&);
    friend double cos(const angle_r&);
    friend double tan(const angle_r&);
    friend angle_r asin_rad(const double&);
    friend angle_r acos_rad(const double&);
    friend angle_r atan_rad(const double&);
    friend angle_r atan2_rad(const double&, const double&);

    friend angle_r ceil(const angle_r&);
    friend angle_r floor(const angle_r&);
    friend angle_r trunc(const angle_r&);
    friend angle_r round(const angle_r&);
    friend angle_r rint(const angle_r&);

    friend angle_r abs(const angle_r&); 

    friend angle_r fdim(const angle_r&, const angle_r&);
    friend angle_r fdim(const angle_r&, const double&);
    friend angle_r fdim(const angle_r&, const angle_d&);
    friend angle_r max(const angle_r&, const angle_r&);
    friend angle_r max(const angle_r&, const double&);
    friend angle_r max(const angle_r&, const angle_d&);
    friend angle_r min(const angle_r&, const angle_r&);
    friend angle_r min(const angle_r&, const double&);
    friend angle_r min(const angle_r&, const angle_d&);
    
    angle_r normalized() const;
    void normalize();


private:

    double m_angle;
};

inline angle_r abs(const angle_r& t_angle) {
    return angle_r(std::abs(t_angle.m_angle));
}

inline void angle_r::normalize() {
    m_angle = std::fmod(m_angle, 2*PI);
}

inline angle_r angle_r::normalized() const {
    angle_r ang(m_angle);
    ang.normalize();
    return ang;
}

inline double sin(const angle_r& t_angle) {
    return std::sin(t_angle.normalized().m_angle);
}
inline double cos(const angle_r& t_angle) {
    return std::cos(t_angle.normalized().m_angle);
}
inline double tan(const angle_r& t_angle) {
    return std::tan(t_angle.normalized().m_angle);
}
inline angle_r asin_rad(const double& t_x) {
    return angle_r(std::asin(t_x));
}
inline angle_r acos_rad(const double& t_x) {
    return angle_r(std::acos(t_x));
}
inline angle_r atan_rad(const double& t_x) {
    return angle_r(std::atan(t_x));
}
inline angle_r atan2_rad(const double& t_x, const double& t_y) {
    return angle_r(std::atan2(t_x, t_y));
}

inline angle_r ceil(const angle_r& t_angle) {
    return angle_r(std::ceil(t_angle.m_angle));
}
inline angle_r floor(const angle_r& t_angle) {
    return angle_r(std::floor(t_angle.m_angle));
}
inline angle_r trunc(const angle_r& t_angle) {
    return angle_r(std::trunc(t_angle.m_angle));
}
inline angle_r round(const angle_r& t_angle) {
    return angle_r(std::round(t_angle.m_angle));
}
inline angle_r rint(const angle_r& t_angle) {
    return angle_r(std::rint(t_angle.m_angle));
}

inline angle_r fdim(const angle_r& t_a, const angle_r& t_b) {
    return std::fdim(t_a.m_angle, t_b.m_angle);
}
inline angle_r fdim(const angle_r& t_a, const double& t_b) {
    return std::fdim(t_a.m_angle, t_b);
}
inline angle_r max(const angle_r& t_a, const angle_r& t_b) {
    return std::max(t_a.m_angle, t_b.m_angle);
}
inline angle_r max(const angle_r& t_a, const double& t_b) {
    return std::max(t_a.m_angle, t_b);
}
inline angle_r min(const angle_r& t_a, const angle_r& t_b) {
    return std::min(t_a.m_angle, t_b.m_angle);
}
inline angle_r min(const angle_r& t_a, const double& t_b) {
    return std::min(t_a.m_angle, t_b);
}

}

#endif
