#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

#include <cmath>

namespace util {

/**
* @brief PI static constant
*/
inline static const double PI      = 3.1415926535897932;
/**
* @brief E static constant
*/
inline static const double E       = 2.7182818284590452;
/**
* @brief Epsilon static constant
*/
inline static const double EPSILON = 0.00001;

/**
* @brief f(loat) eq(ual). checks if two floats are equal within a certain tollarance (epsilon)
*
* @param t_lhs the first float to compare
* @param t_rhs the second float to compare
* @param t_epsilon the tolarance of the comparision
*
* @return if `t_lhs` and `t_rhs` are equal within `t_epsilon`
*/
inline bool feq(const float& t_lhs, const float& t_rhs, const double& t_epsilon = EPSILON) {
    return (std::abs(t_lhs - t_rhs) < t_epsilon);
}

/**
* @brief d(ouble) eq(ual). checks if two doubles are equal within a certain persition (epsilon)
*
* @param t_lhs first double to compare
* @param t_rhs the second double to compare
* @param t_epsilon the tolarance of the comparision
*
* @return if `t_lhs` and `t_rhs` are equal within `t_epsilon`
*/
inline bool deq(const double& t_lhs, const double& t_rhs, const double& t_epsilon = EPSILON) {
    return (std::abs(t_lhs - t_rhs) < t_epsilon);
}

}

#endif
