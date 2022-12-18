#ifndef UTIL_ANGLE_ANGLE_CONV_HPP
#define UTIL_ANGLE_ANGLE_CONV_HPP

#include "angle_op.hpp"
#include "../math.hpp"

namespace util {

template<>
inline util::angle<util::angle_unit::radian> util::angle<util::angle_unit::degree>::to() const {
    return angle_r((m_angle * util::PI) / 180.);
}

template<>
inline util::angle<util::angle_unit::degree> util::angle<util::angle_unit::radian>::to() const {
    return angle_d((m_angle * 180.) / util::PI);
}
    
// * angle_r to angle_d convs
inline angle_r::angle(const angle_d& t_angle) {
    m_angle = t_angle.to<angle_unit::radian>().get();
}

inline angle_r& angle_r::operator+=(const angle_d& t_angle) {
    *(this) += t_angle.to<angle_unit::radian>();
    return *(this);
}
inline angle_r& angle_r::operator-=(const angle_d& t_angle) {
    *(this) -= t_angle.to<angle_unit::radian>();
    return *(this);
}
inline angle_r& angle_r::operator*=(const angle_d& t_angle) {
    *(this) *= t_angle.to<angle_unit::radian>();
    return *(this);
}
inline angle_r& angle_r::operator/=(const angle_d& t_angle) {
    *(this) *= t_angle.to<angle_unit::radian>();
    return *(this);
}

inline angle_r operator+(const angle_r& t_lhs, const angle_d& t_rhs) {
    angle_r result(t_lhs);
    result += t_rhs;
    return result;
}
inline angle_r operator-(const angle_r& t_lhs, const angle_d& t_rhs) {
    angle_r result(t_lhs);
    result -= t_rhs;
    return result;
}
inline angle_r operator*(const angle_r& t_lhs, const angle_d& t_rhs) {
    angle_r result(t_lhs);
    result *= t_rhs;
    return result;
}
inline angle_r operator/(const angle_r& t_lhs, const angle_d& t_rhs) {
    angle_r result(t_lhs);
    result /= t_rhs;
    return result;
}

inline bool operator==(const angle_r&t_lhs, const angle_d&t_rhs) {
    angle_r rhs(t_rhs);
    return (util::deq(t_lhs.m_angle, rhs.m_angle));
}
inline bool operator!=(const angle_r&t_lhs, const angle_d&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator>(const angle_r&t_lhs, const angle_d&t_rhs) {
    angle_r rhs(t_rhs);
    return (t_lhs.m_angle < rhs.m_angle);
}
inline bool operator<(const angle_r&t_lhs, const angle_d&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<=(const angle_r&t_lhs, const angle_d&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator>=(const angle_r&t_lhs, const angle_d&t_rhs) {
    return (t_lhs < t_rhs);
}


// * angle_d to angle_r convs
inline angle_d::angle(const angle_r& t_angle) {
    m_angle = t_angle.to<angle_unit::degree>().get();
}

inline angle_d& angle_d::operator+=(const angle_r& t_angle) {
    *(this) += t_angle.to<angle_unit::degree>();
    return *(this);
}
inline angle_d& angle_d::operator-=(const angle_r& t_angle) {
    *(this) -= t_angle.to<angle_unit::degree>();
    return *(this);
}
inline angle_d& angle_d::operator*=(const angle_r& t_angle) {
    *(this) *= t_angle.to<angle_unit::degree>();
    return *(this);
}
inline angle_d& angle_d::operator/=(const angle_r& t_angle) {
    *(this) *= t_angle.to<angle_unit::degree>();
    return *(this);
}

inline angle_d operator+(const angle_d& t_lhs, const angle_r& t_rhs) {
    angle_d result(t_lhs);
    result += t_rhs;
    return result;
}
inline angle_d operator-(const angle_d& t_lhs, const angle_r& t_rhs) {
    angle_d result(t_lhs);
    result -= t_rhs;
    return result;
}
inline angle_d operator*(const angle_d& t_lhs, const angle_r& t_rhs) {
    angle_d result(t_lhs);
    result *= t_rhs;
    return result;
}
inline angle_d operator/(const angle_d& t_lhs, const angle_r& t_rhs) {
    angle_d result(t_lhs);
    result /= t_rhs;
    return result;
}

inline bool operator==(const angle_d&t_lhs, const angle_r&t_rhs) {
    angle_d rhs(t_rhs);
    return (util::deq(t_lhs.m_angle, rhs.m_angle));
}
inline bool operator!=(const angle_d&t_lhs, const angle_r&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator>(const angle_d&t_lhs, const angle_r&t_rhs) {
    angle_d rhs(t_rhs);
    return (t_lhs.m_angle < rhs.m_angle);
}
inline bool operator<(const angle_d&t_lhs, const angle_r&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<=(const angle_d&t_lhs, const angle_r&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator>=(const angle_d&t_lhs, const angle_r&t_rhs) {
    return (t_lhs < t_rhs);
}

}

#endif
