#ifndef UTIL_UBINT_HPP
#define UTIL_UBINT_HPP

#include <iostream>
#include <vector>
#include "serializable_string.hpp"

namespace util
{
    /**
     * @brief unsinged big int. ints are stored as a list of chars
     * 
     */
    class ubint : public serializable
    {
    public:

        /**
         * @brief Construct a new ubint object from int
         * 
         * @param t_n construct a new ubint with this value
         */
        ubint(const unsigned long long& t_n = 0)
        {
            unsigned long long n = t_n;
            do {
                m_digits.push_back(n % 10);
                n /= 10;
            } while (n);
        }
        /**
         * @brief Construct a new ubint object from string
         * 
         * @param t_s string representation of the number
         */
        ubint(const std::string& t_s)
        {
            m_digits = "";
            for (int i = t_s.size() - 1; i >= 0; i--)
            {
                if (!isdigit(t_s[i]))
                {
                    throw std::invalid_argument("input must be a number");
                }

                m_digits.push_back(t_s[i] - '0');
            }
        }
        /**
         * @brief Construct a new ubint object from const char*
         * 
         * @param t_s string representation of the number
         */
        ubint(const char* t_s) : ubint(std::string(t_s)) {}
        /**
         * @brief Construct a new ubint object from ubint
         * 
         * @param t_u ubint to copy
         */
        ubint(const ubint& t_u) : m_digits(t_u.m_digits) {}
        /**
         * @brief Construct a new ubint object from ubint
         * 
         * @param t_u ubint to move
         */
        ubint(ubint&& t_u) : m_digits(std::move(t_u.m_digits)) {}

        /**
         * @brief ubint assignment operator
         * 
         * @param t_other other ubint to construct ubint from
         * @return ubint& 
         */
        ubint& operator=(const ubint& t_other) { m_digits = t_other.m_digits; return (*this); }
        /**
         * @brief ubint move assignment operator
         * 
         * @param t_other other ubint to move ubint data from
         * @return ubint& 
         */
        ubint& operator=(ubint&& t_other) { m_digits = std::move(t_other.m_digits); return (*this); }

        /**
         * @brief serializanle read. reads the byte stream from file
         * 
         * @param t_stream the input file stream to read from
         */
        void read(std::ifstream &t_stream) final override
        {
            serializable_string s;
            s.read(t_stream);
            m_digits = s.string();
        }

        /**
         * @brief seriazable write. writes the digits to file
         * 
         * @param t_stream 
         */
        void write(std::ofstream &t_stream) const final override
        {
            serializable_string s = m_digits;
            s.write(t_stream);
        }

        /**
         * @brief ubint [] operator
         * 
         * @param t_i get t_i'th digit
         * @return int 
         */
        int operator[](const int& t_i) const
        {
            if (m_digits.size() <= t_i || t_i < 0)
            {
                throw std::out_of_range("index out of range");
            }

            return m_digits[t_i];
        }
        /**
         * @brief ubint [] & operator
         * 
         * @param t_i get t_i'th digit
         * @return int 
         */
        char& operator[](const int& t_i)
        {
            if (m_digits.size() <= t_i || t_i < 0)
            {
                throw std::out_of_range("index out of range");
            }

            return m_digits[t_i];
        }
        /**
         * @brief get string representation of ubint
         * 
         * @return std::string 
         */
        std::string value() const
        {
            std::string val;
            for (int i = 0; i < m_digits.size(); i++)
            {
                val.insert(0, std::to_string(m_digits[i]));
            }

            return val;
        }

        /**
         * @brief ubint ++ operator
         * 
         * @return ubint& 
         */
        ubint& operator++()
        {
            int i,n = m_digits.size();

            for (i = 0; i < n && m_digits[i] == 9; i++)
            {
                m_digits[i] = 0;
            }

            if (i == n)
            {
                m_digits.push_back(1);
            }
            else
            {
                m_digits[i]++;
            }

            return (*this);
        }
        /**
         * @brief ubint ++(int) operator
         * 
         * @param t_n ammount to repete increment (0 default)
         * @return ubint 
         */
        ubint operator++(int t_n)
        {
            ubint result = *this;

            if (t_n != 0)
            {
                (*this) += t_n;
            }
            else
            {
                ++(*this);
            }

            return result;
        }
        /**
         * @brief ubint -- operator
         * 
         * @return ubint& 
         */
        ubint& operator--()
        {
            if (null(*this))
            {
                throw std::underflow_error("underflow");
            }

            int i, n = m_digits.size();
            for (i = 0; i < n && m_digits[i] == 0; i++)
            {
                m_digits[i] = 9;
            }

            m_digits[i]--;
            if (n > 1 && m_digits[n - 1] == 0)
            {
                m_digits.pop_back();
            }

            return *this;
        }
        /**
         * @brief ubint -- operator
         * 
         * @param t_n ammount to repete decrement (0 default)
         * @return ubint 
         */
        ubint operator--(int t_n)
        {
            ubint result = *this;

            if (t_n != 0)
            {
                (*this) -= t_n;
            }
            else
            {
                --(*this);
            }

            return result;
        }

        /**
         * @brief check if a ubint is null
         * 
         * @param t_u the ubint to check if null
         * @return true if ubint is length 1 and only char is 0
         * @return false otherwise
         */
        friend bool null(const ubint&);
        /**
         * @brief gets the length of a ubint
         * 
         * @param t_u the ubint to get length for
         * @return int 
         */
        friend int length(const ubint&);

        /**
         * @brief ubint += operator
         * 
         * @param t_a lhs ubint of +=
         * @param t_b rhs ubint of +=
         * @return ubint& 
         */
        friend ubint &operator+=(ubint&, const ubint&);
        /**
         * @brief ubint + operator
         * 
         * @param t_a lhs ubint of +
         * @param t_b rhs ubint of +
         * @return ubint
         */
        friend ubint operator+(const ubint&, const ubint&);
        /**
         * @brief ubint -= operator
         * 
         * @param t_a lhs ubint of -=
         * @param t_b rhs ubint of -=
         * @return ubint& 
         */
        friend ubint &operator-=(ubint&, const ubint&);
        /**
         * @brief ubint - operator
         * 
         * @param t_a lhs ubint of -
         * @param t_b rhs ubint of -
         * @return ubint 
         */
        friend ubint operator-(const ubint&, const ubint&);

        /**
         * @brief ubint *= operator
         * 
         * @param t_a lhs ubint of *=
         * @param t_b rhs ubint of *=
         * @return ubint& 
         */
        friend ubint &operator*=(ubint&, const ubint&);
        /**
         * @brief ubint * operator
         * 
         * @param t_a ubint lhs of *
         * @param t_b ubint rhs of *
         * @return ubint 
         */
        friend ubint operator*(const ubint&, const ubint&);
        /**
         * @brief ubint /= operator
         * 
         * @param t_a ubint lhs of /=
         * @param t_b ubint rhs of /=
         * @return ubint& 
         */
        friend ubint &operator/=(ubint&, const ubint&);
        /**
         * @brief ubint / operator
         * 
         * @param t_a ubint lhs of /
         * @param t_b ubint rhs of /
         * @return ubint 
         */
        friend ubint operator/(const ubint&, const ubint&);

        /**
         * @brief ubint modulo and assign operator
         * 
         * @param t_a ubint %= lhs
         * @param t_b ubint %= rhs
         * @return ubint 
         */
        friend ubint operator%=(ubint&, const ubint&);
        /**
         * @brief ubint modulo
         * 
         * @param t_a ubint modulo lhs
         * @param t_b ubint modulo rhs
         * @return ubint 
         */
        friend ubint operator%(const ubint&, const ubint&);

        /**
         * @brief ubint left shift and assign
         * 
         * @param t_a ubint left shift lhs
         * @param t_b ubint left shift rhs
         * @return ubint& 
         */
        friend ubint &operator<<=(ubint&, const ubint&);
        /**
         * @brief ubint left shift
         * 
         * @param t_a ubint left shift lhs
         * @param t_b ubint left shift rhs
         * @return ubint 
         */
        friend ubint operator<<(const ubint&, const ubint&);
        /**
         * @brief ubint right shift and assign
         * 
         * @param t_a ubint right shift lhs
         * @param t_b ubint right shift rhs
         * @return ubint& 
         */
        friend ubint &operator>>=(ubint&, const ubint&);
        /**
         * @brief ubint right shift
         * 
         * @param t_a ubint right shift lhs
         * @param t_t ubint right shift rhs
         * @return ubint 
         */
        friend ubint operator>>(const ubint&, const ubint&);

        /**
         * @brief ubint exponent and assign
         * 
         * @param t_a ubint exponent base
         * @param t_b ubint exponent power
         * @return ubint& 
         */
        friend ubint &operator^=(ubint&, const ubint&);
        /**
         * @brief ubint exponent
         * 
         * @param t_a ubint exponent base
         * @param t_b ubint exponent power
         * @return ubint 
         */
        friend ubint operator^(const ubint&, const ubint&);

        /**
         * @brief ubint == comparison
         * 
         * @param t_a ubint == lhs
         * @param t_b ubint == rhs
         * @return true if t_a is equal to t_b
         * @return false otherwise
         */
        friend bool operator==(const ubint&, const ubint&);
        /**
         * @brief ubint != comparison
         * 
         * @param t_a ubint != lhs
         * @param t_b ubint != rhs
         * @return true if t_a is not equal to t_b
         * @return false otherwise
         */
        friend bool operator!=(const ubint&, const ubint&);
        /**
         * @brief ubint grater than operator
         * 
         * @param t_a ubint grather than lhs
         * @param t_b ubint grather than rhs
         * @return true if t_a > t_b
         * @return false otherwise
         */
        friend bool operator>(const ubint&, const ubint&);
        /**
         * @brief ubint grater than or equal to operator
         * 
         * @param t_a ubint >= lhs
         * @param t_b ubint >= rhs
         * @return true if t_a >= t_b
         * @return false otherwise
         */
        friend bool operator>=(const ubint&, const ubint&);
        /**
         * @brief ubint less than operator
         * 
         * @param t_a ubint less than lhs
         * @param t_b ubint less than rhs
         * @return true if t_a is less than t_b
         * @return false otherwise
         */
        friend bool operator<(const ubint&, const ubint&);
        /**
         * @brief ubint less than or equal to operator
         * 
         * @param t_a ubint <= lhs
         * @param t_b ubint <= rhs
         * @return true if t_a <= t_b
         * @return false otherwise
         */
        friend bool operator<=(const ubint&, const ubint&);

        /**
         * @brief ubint squere root
         * 
         * @param t_n the ubint to compute the squere root of
         * @return ubint 
         */
        friend ubint sqrt(const ubint&);
        /**
         * @brief ubint min. finds the minumum value of two ubints
         * 
         * @param t_a ubint min arg1
         * @param t_b ubint min arg2
         * @return ubint 
         */
        friend ubint min(const ubint&, const ubint&);
        /**
         * @brief ubint max. finds the maximum value of two ubints
         * 
         * @param t_a ubint max arg1
         * @param t_b ubint max arg2
         * @return ubint 
         */
        friend ubint max(const ubint&, const ubint&);
        /**
         * @brief ubint factorial. computes n! or n(n-1)(n-2)(n-3)...(1)
         * 
         * @param t_n computes t_n!
         * @return ubint 
         */
        friend ubint factorial(const ubint&);
        /**
         * @brief ubint map. maps t_x from the range [t_orig_lower..t_orig_upper] to the range
         * [t_dest_lower..t_dest_upper]
         * 
         * @param t_x the ubint to be mapped to the new range
         * @param t_orig_lower the lower ubint from the old range
         * @param t_orig_upper the upper ubint from the old range
         * @param t_dest_lower the lower ubint for the new range
         * @param t_dest_upper the upper ubint for the new range
         * @return ubint 
         */
        friend ubint map(const ubint&, const ubint&, const ubint&, const ubint&, const ubint&);

        /**
         * @brief ubint standard out
         * 
         * @param t_out output stream to write ubint to
         * @param t_x ubint to write
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream&, const ubint&);
        /**
         * @brief ubint standard in
         * 
         * @param t_in input stream to read ubint from
         * @param t_x ubint to read
         * @return std::istream& 
         */
        friend std::istream& operator>>(std::istream&, ubint&);


    private:

        std::string m_digits;
    };

    bool null(const ubint& t_u)
    {
        return (t_u.m_digits.size() == 1 && t_u.m_digits[0] == 0);
    }

    int length(const ubint& t_u)
    {
        return t_u.m_digits.size();
    }

    ubint &operator+=(ubint& t_a, const ubint& t_b)
    {
        // * t = carry, s = sum, i = index, n = length(a), m = length(b)
        int t = 0, s, i;
        int n = length(t_a), m = length(t_b);

        if (m > n)
        {
            t_a.m_digits.append(m - n, 0);
            n = length(t_a);
        }

        for (i = 0; i < n; i++)
        {
            if (i < m)
            {
                s = (t_a.m_digits[i] + t_b.m_digits[i]) + t;
            }
            else
            {
                s = (t_a.m_digits[i]) + t;
            }
            t = s / 10;
            t_a.m_digits[i] = (s % 10);
        }

        if (t)
        {
            t_a.m_digits.push_back(t);
        }

        return t_a;
    }

    ubint operator+(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp += t_b;
        return tmp;
    }

    ubint &operator-=(ubint& t_a, const ubint& t_b)
    {
        if (t_a < t_b)
        {
            throw std::underflow_error("underflow. a cannot be less than b");
        }

        int n = length(t_a), m = length(t_b);
        int i, t = 0, s;

        for (i = 0; i < n; i++)
        {
            if (i < m)
            {
                s = (t_a.m_digits[i] - t_b.m_digits[i]) + t;
            }
            else
            {
                s = t_a.m_digits[i] + t;
            }

            if (s < 0)
            {
                s += 10;
                t = -1;
            }
            else
            {
                t = 0;
            }

            t_a.m_digits[i] = s;
        }

        while (n > 1 && t_a.m_digits[n - 1] == 0)
        {
            t_a.m_digits.pop_back();
            n--;
        }

        return t_a;
    }

    ubint operator-(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp -= t_b;
        return tmp;
    }

    ubint &operator*=(ubint& t_a, const ubint& t_b)
    {
        if (null(t_a) || null(t_b))
        {
            t_a = ubint();
            return t_a;
        }

        int n = length(t_a), m = length(t_b);
        std::vector<int> v(n + m, 0);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                v[i+j] += (t_a.m_digits[i]) * (t_b.m_digits[j]);
            }
        }
        n += m;
        t_a.m_digits.resize(v.size());
        for (int s, i = 0, t = 0; i < n; i++)
        {
            s = t + v[i];
            v[i] = (s % 10);
            t = s / 10;
            t_a.m_digits[i] = v[i];
        }

        for (int i = n - 1; i >= 1 && !v[i]; i--)
        {
            t_a.m_digits.pop_back();
        }

        return t_a;
    }

    ubint operator*(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp *= t_b;
        return tmp;
    }

    ubint &operator/=(ubint& t_a, const ubint& t_b)
    {
        if (null(t_b))
        {
            throw std::invalid_argument("cannot divide by zero");
        }

        if (null(t_a))
        {
            return t_a;
        }

        if (t_a < t_b)
        {
            t_a = 0ULL;
            return t_a;
        }

        if (t_a == t_b)
        {
            t_a = 1;
            return t_a;
        }

        int i, lgcat = 0, cc;
        int n = length(t_a);
        std::vector<int> cat(n, 0);
        ubint t;
        for (i = n - 1; t * 10 + t_a.m_digits[i] < t_b; i--)
        {
            t *= 10;
            t += t_a.m_digits[i];
        }

        for (; i >= 0; i--)
        {
            t = t * 10 + t_a.m_digits[i];
            for (cc = 9; cc * t_b > t; cc--);
            t -= cc * t_b;
            cat[lgcat++] = cc;
        }

        t_a.m_digits.resize(cat.size());
        for (i = 0; i < lgcat; i++)
        {
            t_a.m_digits[i] = cat[lgcat - i - 1];
        }
        t_a.m_digits.resize(lgcat);

        return t_a;
    }

    ubint operator/(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp /= t_b;
        return tmp;
    }

    ubint operator%=(ubint& t_a, const ubint& t_b)
    {
        if (null(t_b))
        {
            throw std::invalid_argument("cannot divide by zero");
        }

        if (t_b == 1 || t_a == t_b)
        {
            t_a = 0ULL;
            return t_a;
        }

        if (t_a < t_b)
        {
            return t_a;
        }

        int i, lgcat = 0, cc;
        int n = length(t_a);
        std::vector<int> cat(n, 0);
        ubint t;
        for (i = n - 1; t * 10 + t_a.m_digits[i] < t_b; i--)
        {
            t *= 10;
            t += t_a.m_digits[i];
        }

        for (; i >= 0; i--)
        {
            t = t * 10 + t_a.m_digits[i];
            for (cc = 9; cc * t_b > t;cc--);
            t -= cc * t_b;
            cat[lgcat++] = cc;
        }

        t_a = t;
        return t_a;
    }

    ubint operator%(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp %= t_b;
        return tmp;
    }

    ubint &operator<<=(ubint& t_a, const ubint& t_b)
    {
        for (ubint i = 0ULL; i < t_b; i++)
        {
            t_a += t_a;
        }

        return t_a;
    }
        
    ubint operator<<(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp <<= t_b;
        return tmp;
    }
    
    ubint &operator>>=(ubint& t_a, const ubint& t_b)
    {
        for (ubint i = 0ULL; i < t_b; i++)
        {
            int add = 0;
            for (int i = t_a.m_digits.size() - 1; i >= 0; i--)
            {
                int digit = (t_a.m_digits[i] >> 1) + add;
                add = ((t_a.m_digits[i] & 1) * 5);
                t_a.m_digits[i] = digit;
            }

            while(t_a.m_digits.size() > 1 && !t_a.m_digits.back())
            {
                t_a.m_digits.pop_back();
            }
        }

        return t_a;
    }
    
    ubint operator>>(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp >>= t_b;
        return tmp;
    }

    ubint &operator^=(ubint& t_a, const ubint& t_b)
    {
        ubint exp = t_b, base = t_a;
        t_a = 1;

        while (!null(exp))
        {
            if (exp[0] & 1)
            {
                t_a *= base;
            }

            base *= base;
            exp >>= 1;
        }

        return t_a;
    }

    ubint operator^(const ubint& t_a, const ubint& t_b)
    {
        ubint tmp = t_a;
        tmp ^= t_b;
        return tmp;
    }

    bool operator==(const ubint& t_a, const ubint& t_b)
    {
        return t_a.m_digits == t_b.m_digits;
    }

    bool operator!=(const ubint& t_a, const ubint& t_b)
    {
        return !(t_a == t_b);
    }

    bool operator>(const ubint& t_a, const ubint& t_b)
    {
        return t_b < t_a;
    }

    bool operator>=(const ubint& t_a, const ubint& t_b)
    {
        return !(t_a < t_b);
    }

    bool operator<(const ubint& t_a, const ubint& t_b)
    {
        int n = length(t_a), m = length(t_b);
        if (n != m)
        {
            return n < m;
        }

        while (n--)
        {
            if (t_a.m_digits[n] != t_b.m_digits[n])
            {
                return t_a.m_digits[n] < t_b.m_digits[n];
            }
        }

        return false;
    }

    bool operator<=(const ubint& t_a, const ubint& t_b)
    {
        return !(t_a > t_b);
    }

    ubint sqrt(const ubint& t_n)
    {
        ubint left = 1, right = t_n, v = 1, mid, prod;
        right >>= 1;

        while(left <= right)
        {
            mid += left;
            mid += right;
            mid >>= 1;
            prod = (mid * mid);
            if (prod <= t_n)
            {
                v = mid;
                ++mid;
                left = mid;
            } 
            else
            {
                --mid;
                right = mid;
            }
            mid = 0ULL;
        }

        return v;
    }

    ubint min(const ubint& t_a, const ubint& t_b)
    {
        return (t_a < t_b) ? t_a : t_b;
    }

    ubint max(const ubint& t_a, const ubint& t_b)
    {
        return (t_a > t_b) ? t_a : t_b;
    }

    ubint factorial(const ubint& t_n)
    {
        if (t_n == 1)
        {
            return 1;
        }

        return t_n * factorial(t_n - 1);
    }

    ubint map(const ubint& t_x, 
        const ubint& t_orig_lower,
        const ubint& t_orig_upper,
        const ubint& t_dest_lower,
        const ubint& t_dest_upper)
    {
        return (t_x - t_orig_lower) * 
            (t_dest_upper - t_dest_lower) / (t_orig_upper - t_orig_lower)
             + t_dest_lower;
    }

    std::ostream& operator<<(std::ostream& t_out, const ubint& t_x)
    {
        for (int i = t_x.m_digits.size() - 1; i >= 0; i--)
        {
            t_out << (short)t_x.m_digits[i];
        }

        return t_out;
    }

    std::istream& operator>>(std::istream& t_in, ubint& t_x)
    {
        std::string s;
        t_in >> s;
        int n = s.size();

        for (int i = n - 1; i >= 0; i--)
        {
            if (!isdigit(s[i]))
            {
                throw std::invalid_argument("input must be a number");
            }

            t_x.m_digits[n - i - 1] = s[i];
        }

        return t_in;
    }
}

#endif