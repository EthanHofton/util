#ifndef UTIL_ANGLE_ANGLE_DEG_HPP
#define UTIL_ANGLE_ANGLE_DEG_HPP

#include "angle_def.hpp"

namespace util {

template<>
class angle<angle_unit::degree> {
public:

    // * constructors *
    angle(const double& t_angle) : m_angle(t_angle) {}
    angle(const angle_d& t_angle) : m_angle(t_angle.m_angle) {}
    angle(const angle_r&);

    // * getters *
    double get() const { return m_angle; }
    friend angle_d abs(const angle_d&);

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


private:

    double m_angle;
};

inline angle_d abs(const angle_d& t_angle) {
    return angle_d(std::abs(t_angle.m_angle));
}

}

#endif
