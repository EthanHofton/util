#ifndef UTIL_ALGO_HPP
#define UTIL_ALGO_HPP

#include <string>
#include <sstream>

namespace util
{
    /**
     * @brief generic min function. computes the min out of two values
     * 
     * @tparam T the type of the two values
     * @param t_a min arg1
     * @param t_b min arg2
     * @return T 
     */
    template<typename T>
    T min(const T& t_a, const T& t_b)
    {
        return (t_a < t_b) ? t_a : t_b;
    }

    /**
     * @brief generic max function. computes the max out of two values
     * 
     * @tparam T the type of the two values
     * @param t_a max arg1
     * @param t_b max arg2
     * @return T 
     */
    template<typename T>
    T max(const T& t_a, const T& t_b)
    {
        return (t_a > t_b) ? t_a : t_b;
    }

    /**
     * @brief clamp function. clamps t_x between the values t_min and t_max
     * 
     * @tparam T the type of t_x
     * @param t_x the value to be clamped
     * @param t_min clamp lower bound
     * @param t_max clamp upper bound
     * @return T 
     */
    template<typename T>
    T clamp(const T& t_x, const T& t_min, const T& t_max)
    {
        max<T>(t_min, min<T>(t_x, t_max));
    }

    /**
     * @brief compute the absoulte value. 
     * 
     * @tparam T the type of t_a
     * @param t_a the value to compute the absolute value for
     * @return T 
     */
    template<typename T>
    T abs(const T& t_a)
    {
        (t_a >= 0) ? t_a : -t_a;
    }

    /**
     * @brief computes the gratest common devisor of t_a and t_b (using euclidean algorithm)
     * 
     * @tparam T the type of t_a and t_b
     * @param t_a gcd arg 1
     * @param t_b gcd arg 2
     * @return T 
     */
    template<typename T>
    T gcd(const T& t_a, const T& t_b)
    {
        if (t_b == 0)
        {
            return t_a;
        }

        return gcd<T>(t_b, t_a % t_b);
    }

    /**
     * @brief computes the lowest common multiple of t_a and t_b (using gcd definition)
     * 
     * @tparam T the type of t_a and t_b
     * @param t_a lcm arg 1
     * @param t_b lcm arg 2
     * @return T 
     */
    template<typename T>
    T lcm(const T& t_a, const T& t_b)
    {
        return (abs<T>(t_a * t_b)) / gcd(t_a, t_b);
    }

    /**
     * @brief format base case
     * 
     * @param t_fmt the format string to return
     * @return std::string 
     */
    std::string fmt(std::string t_fmt)
    {
        return t_fmt;
    }

    /**
     * @brief format a string, replace {} with arguments provided in squentail order
     * 
     * @tparam T the type of the current argument
     * @tparam Args the rest of the arguments
     * @param t_fmt the format string
     * @param t_val the top of the arugment list
     * @param t_rest the rest of the argument list
     * @return std::string 
     */
    template<typename T, typename... Args>
    std::string fmt(std::string t_fmt, T t_val, Args... t_rest)
    {
        size_t pos = t_fmt.find("{}");
        if (pos == std::string::npos) throw std::invalid_argument("too many arguments provided");
        std::stringstream buf;
        buf << t_val;
        t_fmt.replace(t_fmt.begin() + pos, t_fmt.begin() + pos + 2, buf.str());
        return fmt(t_fmt, t_rest...);
    }

    /**
     * @brief takes an format string and replaces the corrisponding placeholders with there values
     * 
     * @param fmt format string describing what time to use
     * @return std::string 
     */
    std::string strfmttime(const std::string& fmt)
    {
        char outstr[200];
        time_t t = time(NULL);
        struct tm *tmp = gmtime(&t);
        strftime(outstr, sizeof(outstr), fmt.c_str(), tmp);
        return std::string(outstr);
    }
}

#endif