#ifndef UTIL_ANGLE_ANGLE_CONV_HPP
#define UTIL_ANGLE_ANGLE_CONV_HPP

#include "angle_op.hpp"
#include "../math.hpp"

namespace util {

template<>
inline angle_r util::angle_d::to() const {
    return angle_r((m_angle * util::PI) / 180.);
}

template<>
inline angle_d util::angle_r::to() const {
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

// * angle_r math funcs
inline angle_r fdim(const angle_r& t_a, const angle_d& t_b) {
    angle_r BasR(t_b);
    return util::fdim(t_a, BasR);
}
inline angle_r max(const angle_r& t_a, const angle_d& t_b) {
    angle_r BasR(t_b);
    return util::max(t_a, BasR);
}
inline angle_r min(const angle_r& t_a, const angle_d& t_b) {
    angle_r BasR(t_b);
    return util::min(t_a, BasR);
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

// * angle_d math functions
inline double sin(const angle_d& t_angle) {
    return std::sin(t_angle.to<angle_unit::radian>().normalized().get());
}
inline double cos(const angle_d& t_angle) {
    return std::cos(t_angle.to<angle_unit::radian>().normalized().get());
}
inline double tan(const angle_d& t_angle) {
    return std::tan(t_angle.to<angle_unit::radian>().normalized().get());
}
inline angle_d asin_deg(const double& t_x) {
    return angle_r(std::asin(t_x)).to<angle_unit::degree>();
}
inline angle_d acos_deg(const double& t_x) {
    return angle_r(std::acos(t_x)).to<angle_unit::degree>();
}
inline angle_d atan_deg(const double& t_x) {
    return angle_r(std::atan(t_x)).to<angle_unit::degree>();
}
inline angle_d atan2_deg(const double& t_x, const double& t_y) {
    return angle_r(std::atan2(t_x, t_y)).to<angle_unit::degree>();
}

inline angle_d fdim(const angle_d& t_a, const angle_r& t_b) {
    angle_d BasD(t_b);
    return util::fdim(t_a, BasD);
}
inline angle_d max(const angle_d& t_a, const angle_r& t_b) {
    angle_d BasD(t_b);
    return util::max(t_a, BasD);
}
inline angle_d min(const angle_d& t_a, const angle_r& t_b) {
    angle_d BasD(t_b);
    return util::min(t_a, BasD);
}

}

#endif
