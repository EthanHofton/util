#ifndef UTIL_ANGLE_ANGLE_RAD_HPP
#define UTIL_ANGLE_ANGLE_RAD_HPP

#include "angle_def.hpp"

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
    friend  angle_r abs(const angle_r&); 

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


private:

    double m_angle;
};

inline angle_r abs(const angle_r& t_angle) {
    return angle_r(std::abs(t_angle.m_angle));
}

}

#endif
