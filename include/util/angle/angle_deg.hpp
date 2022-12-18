#ifndef UTIL_ANGLE_ANGLE_DEG_HPP
#define UTIL_ANGLE_ANGLE_DEG_HPP

#include "angle_def.hpp"
#include <cmath>

namespace util {

template<>
class angle<angle_unit::degree> {
public:

    // * constructors *
    angle(const double& t_angle) : m_angle(t_angle) {}
    angle(const angle_d& t_angle) : m_angle(t_angle.m_angle) {}
    angle(const angle_r&);

    // * getters and setters *
    double get() const { return m_angle; }

    // * operators *
    ANGLE_COUT(degree)

    angle_d operator+() const;
    angle_d operator-() const;
    angle_d& operator++();
    angle_d operator++(int);
    angle_d& operator--();
    angle_d operator--(int);
    angle_d& operator+=(const double&);
    angle_d& operator+=(const angle_d&);
    angle_d& operator+=(const angle_r&);
    angle_d& operator-=(const double&);
    angle_d& operator-=(const angle_d&);
    angle_d& operator-=(const angle_r&);
    angle_d& operator*=(const double&);
    angle_d& operator*=(const angle_d&);
    angle_d& operator*=(const angle_r&);
    angle_d& operator/=(const double&);
    angle_d& operator/=(const angle_d&);
    angle_d& operator/=(const angle_r&);
    friend angle_d operator+(const angle_d&, const angle_d&);
    friend angle_d operator+(const angle_d&, const double&);
    friend angle_d operator+(const angle_d&, const angle_r&);
    friend angle_d operator-(const angle_d&, const angle_d&);
    friend angle_d operator-(const angle_d&, const double&);
    friend angle_d operator-(const angle_d&, const angle_r&);
    friend angle_d operator*(const angle_d&, const angle_d&);
    friend angle_d operator*(const angle_d&, const double&);
    friend angle_d operator*(const angle_d&, const angle_r&);
    friend angle_d operator/(const angle_d&, const double&);
    friend angle_d operator/(const angle_d&, const angle_d&);
    friend angle_d operator/(const angle_d&, const angle_r&);

    friend bool operator==(const angle_d&, const angle_d&);
    friend bool operator==(const angle_d&, const double&);
    friend bool operator==(const angle_d&, const angle_r&);
    friend bool operator!=(const angle_d&, const angle_d&);
    friend bool operator!=(const angle_d&, const double&);
    friend bool operator!=(const angle_d&, const angle_r&);
    friend bool operator<=(const angle_d&, const angle_d&);
    friend bool operator<=(const angle_d&, const double&);
    friend bool operator<=(const angle_d&, const angle_r&);
    friend bool operator>=(const angle_d&, const angle_d&);
    friend bool operator>=(const angle_d&, const double&);
    friend bool operator>=(const angle_d&, const angle_r&);
    friend bool operator>(const angle_d&, const angle_d&);
    friend bool operator>(const angle_d&, const double&);
    friend bool operator>(const angle_d&, const angle_r&);
    friend bool operator<(const angle_d&, const angle_d&);
    friend bool operator<(const angle_d&, const double&);
    friend bool operator<(const angle_d&, const angle_r&);

    // * methods
    ANGLE_TO()
    ANGLE_TO_SELF(degree)

    friend double sin(const angle_d&);
    friend double cos(const angle_d&);
    friend double tan(const angle_d&);
    friend angle_d asin_deg(const double&);
    friend angle_d acos_deg(const double&);
    friend angle_d atan_deg(const double&);
    friend angle_d atan2_deg(const double&, const double&);

    friend angle_d ceil(const angle_d&);
    friend angle_d floor(const angle_d&);
    friend angle_d trunc(const angle_d&);
    friend angle_d round(const angle_d&);
    friend angle_d rint(const angle_d&);

    friend angle_d abs(const angle_d&); 

    friend angle_d fdim(const angle_d&, const angle_d&);
    friend angle_d fdim(const angle_d&, const double&);
    friend angle_d fdim(const angle_d&, const angle_r&);
    friend angle_d max(const angle_d&, const angle_d&);
    friend angle_d max(const angle_d&, const double&);
    friend angle_d max(const angle_d&, const angle_r&);
    friend angle_d min(const angle_d&, const angle_d&);
    friend angle_d min(const angle_d&, const double&);
    friend angle_d min(const angle_d&, const angle_r&);
    
    angle_d normalized() const;
    void normalize();


private:

    double m_angle;
};

inline angle_d abs(const angle_d& t_angle) {
    return angle_d(std::abs(t_angle.m_angle));
}

inline void angle_d::normalize() {
    m_angle = std::fmod(m_angle, 360.);
}

inline angle_d angle_d::normalized() const {
    angle_d ang(m_angle);
    ang.normalize();
    return ang;
}

inline angle_d ceil(const angle_d& t_angle) {
    return angle_d(std::ceil(t_angle.m_angle));
}
inline angle_d floor(const angle_d& t_angle) {
    return angle_d(std::floor(t_angle.m_angle));
}
inline angle_d trunc(const angle_d& t_angle) {
    return angle_d(std::trunc(t_angle.m_angle));
}
inline angle_d round(const angle_d& t_angle) {
    return angle_d(std::round(t_angle.m_angle));
}
inline angle_d rint(const angle_d& t_angle) {
    return angle_d(std::rint(t_angle.m_angle));
}

inline angle_d fdim(const angle_d& t_a, const angle_d& t_b) {
    return std::fdim(t_a.m_angle, t_b.m_angle);
}
inline angle_d fdim(const angle_d& t_a, const double& t_b) {
    return std::fdim(t_a.m_angle, t_b);
}
inline angle_d max(const angle_d& t_a, const angle_d& t_b) {
    return std::max(t_a.m_angle, t_b.m_angle);
}
inline angle_d max(const angle_d& t_a, const double& t_b) {
    return std::max(t_a.m_angle, t_b);
}
inline angle_d min(const angle_d& t_a, const angle_d& t_b) {
    return std::min(t_a.m_angle, t_b.m_angle);
}
inline angle_d min(const angle_d& t_a, const double& t_b) {
    return std::min(t_a.m_angle, t_b);
}

}

#endif
