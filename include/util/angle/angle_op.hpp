#ifndef UTIL_ANGLE_ANGLE_OP_HPP
#define UTIL_ANGLE_ANGLE_OP_HPP

#include "angle_deg.hpp"
#include "angle_rad.hpp"
#include "../math.hpp"

namespace util {

// * cout *
inline std::ostream& operator <<(std::ostream& t_out, const angle<util::angle_unit::radian>& t_angle) {
    t_out << t_angle.m_angle << "rad";
    return t_out;
}

inline std::ostream& operator <<(std::ostream& t_out, const angle<util::angle_unit::degree>& t_angle) {
    t_out << t_angle.m_angle << "°";
    return t_out;
}


// * radian angle operators *
inline angle_r angle_r::operator+() const {
    angle_r result = +(m_angle);
    return result; 
}
inline angle_r angle_r::operator-()  const {
    angle_r result = -(m_angle);
    return result;
}
inline angle_r& angle_r::operator++() {
    m_angle++;
    return *(this);
}
inline angle_r angle_r::operator++(int) {
    angle_r result = *(this);
    m_angle++;
    return result;
}
inline angle_r& angle_r::operator--() {
    m_angle--;
    return *(this);
}
inline angle_r angle_r::operator--(int) {
    angle_r result = *(this);
    m_angle--;
    return result;
}


inline angle_r& angle_r::operator+=(const double& t_angle) {
    m_angle += t_angle;
    return *(this);
}
inline angle_r& angle_r::operator+=(const angle_r& t_angle) {
    m_angle += t_angle.m_angle;
    return *(this);
}
inline angle_r& angle_r::operator-=(const double& t_angle) {
    m_angle -= t_angle;
    return *(this);
}
inline angle_r& angle_r::operator-=(const angle_r& t_angle) {
    m_angle -= t_angle.m_angle;
    return *(this);
}
inline angle_r& angle_r::operator*=(const double& t_angle) {
    m_angle *= t_angle;
    return *(this);
}
inline angle_r& angle_r::operator*=(const angle_r& t_angle) {
    m_angle *= t_angle.m_angle;
    return *(this);
}
inline angle_r& angle_r::operator/=(const double& t_angle) {
    m_angle /= t_angle;
    return *(this);
}
inline angle_r& angle_r::operator/=(const angle_r& t_angle) {
    m_angle /= t_angle.m_angle;
    return *(this);
}

inline angle_r operator+(const angle_r&t_lhs, const angle_r&t_rhs) {
    angle_r result = t_lhs;
    result += t_rhs;
    return result;
}
inline angle_r operator+(const angle_r&t_lhs, const double& t_rhs) {
    angle_r result = t_lhs;
    result += t_rhs;
    return result;
}
inline angle_r operator-(const angle_r&t_lhs, const angle_r&t_rhs) {
    angle_r result = t_lhs;
    result -= t_rhs;
    return result;
}
inline angle_r operator-(const angle_r&t_lhs, const double& t_rhs) {
    angle_r result = t_lhs;
    result -= t_rhs;
    return result;
}
inline angle_r operator*(const angle_r&t_lhs, const angle_r&t_rhs) {
    angle_r result = t_lhs;
    result *= t_rhs;
    return result;
}
inline angle_r operator*(const angle_r&t_lhs, const double& t_rhs) {
    angle_r result = t_lhs;
    result *= t_rhs;
    return result;
}
inline angle_r operator/(const angle_r&t_lhs, const angle_r&t_rhs) {
    angle_r result = t_lhs;
    result /= t_rhs;
    return result;
}
inline angle_r operator/(const angle_r&t_lhs, const double& t_rhs) {
    angle_r result = t_lhs;
    result /= t_rhs;
    return result;
}

inline bool operator==(const angle_r&t_lhs, const angle_r&t_rhs) {
    return (util::deq(t_lhs.m_angle, t_rhs.m_angle));
}
inline bool operator==(const angle_r&t_lhs, const double&t_rhs) {
    angle_r rhs(t_rhs);
    return (util::deq(t_lhs.m_angle, rhs.m_angle));
}
inline bool operator!=(const angle_r&t_lhs, const angle_r&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator!=(const angle_r&t_lhs, const double&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator>(const angle_r&t_lhs, const angle_r&t_rhs) {
    return (t_lhs.m_angle < t_rhs.m_angle);
}
inline bool operator>(const angle_r&t_lhs, const double&t_rhs) {
    angle_r rhs(t_rhs);
    return (t_lhs.m_angle < rhs.m_angle);
}
inline bool operator<(const angle_r&t_lhs, const angle_r&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<(const angle_r&t_lhs, const double&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<=(const angle_r&t_lhs, const angle_r&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator<=(const angle_r&t_lhs, const double&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator>=(const angle_r&t_lhs, const angle_r&t_rhs) {
    return (t_lhs < t_rhs);
}
inline bool operator>=(const angle_r&t_lhs, const double&t_rhs) {
    return (t_lhs < t_rhs);
}

// * degree angle operators *
inline angle_d angle_d::operator+() const {
    angle_d result = +(m_angle);
    return result; 
}
inline angle_d angle_d::operator-()  const {
    angle_d result = -(m_angle);
    return result;
}
inline angle_d& angle_d::operator++() {
    m_angle++;
    return *(this);
}
inline angle_d angle_d::operator++(int) {
    angle_d result = *(this);
    m_angle++;
    return result;
}
inline angle_d& angle_d::operator--() {
    m_angle--;
    return *(this);
}
inline angle_d angle_d::operator--(int) {
    angle_d result = *(this);
    m_angle--;
    return result;
}

inline angle_d& angle_d::operator+=(const double& t_angle) {
    m_angle += t_angle;
    return *(this);
}
inline angle_d& angle_d::operator+=(const angle_d& t_angle) {
    m_angle += t_angle.m_angle;
    return *(this);
}
inline angle_d& angle_d::operator-=(const double& t_angle) {
    m_angle -= t_angle;
    return *(this);
}
inline angle_d& angle_d::operator-=(const angle_d& t_angle) {
    m_angle -= t_angle.m_angle;
    return *(this);
}
inline angle_d& angle_d::operator*=(const double& t_angle) {
    m_angle *= t_angle;
    return *(this);
}
inline angle_d& angle_d::operator*=(const angle_d& t_angle) {
    m_angle *= t_angle.m_angle;
    return *(this);
}
inline angle_d& angle_d::operator/=(const double& t_angle) {
    m_angle /= t_angle;
    return *(this);
}
inline angle_d& angle_d::operator/=(const angle_d& t_angle) {
    m_angle /= t_angle.m_angle;
    return *(this);
}

inline angle_d operator+(const angle_d&t_lhs, const angle_d&t_rhs) {
    angle_d result = t_lhs;
    result += t_rhs;
    return result;
}
inline angle_d operator+(const angle_d&t_lhs, const double& t_rhs) {
    angle_d result = t_lhs;
    result += t_rhs;
    return result;
}
inline angle_d operator-(const angle_d&t_lhs, const angle_d&t_rhs) {
    angle_d result = t_lhs;
    result -= t_rhs;
    return result;
}
inline angle_d operator-(const angle_d&t_lhs, const double& t_rhs) {
    angle_d result = t_lhs;
    result -= t_rhs;
    return result;
}
inline angle_d operator*(const angle_d&t_lhs, const angle_d&t_rhs) {
    angle_d result = t_lhs;
    result *= t_rhs;
    return result;
}
inline angle_d operator*(const angle_d&t_lhs, const double& t_rhs) {
    angle_d result = t_lhs;
    result *= t_rhs;
    return result;
}
inline angle_d operator/(const angle_d&t_lhs, const angle_d&t_rhs) {
    angle_d result = t_lhs;
    result /= t_rhs;
    return result;
}
inline angle_d operator/(const angle_d&t_lhs, const double& t_rhs) {
    angle_d result = t_lhs;
    result /= t_rhs;
    return result;
}

inline bool operator==(const angle_d&t_lhs, const angle_d&t_rhs) {
    return (util::deq(t_lhs.m_angle, t_rhs.m_angle));
}
inline bool operator==(const angle_d&t_lhs, const double&t_rhs) {
    angle_d rhs(t_rhs);
    return (util::deq(t_lhs.m_angle, rhs.m_angle));
}
inline bool operator!=(const angle_d&t_lhs, const angle_d&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator!=(const angle_d&t_lhs, const double&t_rhs) {
    return !(t_lhs == t_rhs);
}
inline bool operator>(const angle_d&t_lhs, const angle_d&t_rhs) {
    return (t_lhs.m_angle < t_rhs.m_angle);
}
inline bool operator>(const angle_d&t_lhs, const double&t_rhs) {
    angle_d rhs(t_rhs);
    return (t_lhs.m_angle < rhs.m_angle);
}
inline bool operator<(const angle_d&t_lhs, const angle_d&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<(const angle_d&t_lhs, const double&t_rhs) {
    return (t_rhs < t_lhs);
}
inline bool operator<=(const angle_d&t_lhs, const angle_d&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator<=(const angle_d&t_lhs, const double&t_rhs) {
    return (t_lhs > t_rhs);
}
inline bool operator>=(const angle_d&t_lhs, const angle_d&t_rhs) {
    return (t_lhs < t_rhs);
}
inline bool operator>=(const angle_d&t_lhs, const double&t_rhs) {
    return (t_lhs < t_rhs);
}

}

#endif
