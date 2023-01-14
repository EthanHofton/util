#ifndef UTIL_BITSTREAM_HPP
#define UTIL_BITSTREAM_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include "ubint.hpp"
#include "serializable_list.hpp"

namespace util
{
    /**
     * @brief a class to store a stream of bits
     * 
     */
    class bitstream : public serializable
    {
    public:

        /**
         * @brief Construct a new bitstream object
         * 
         */
        bitstream() {}
        /**
         * @brief Construct a new bitstream object from string
         * 
         * @param t_str a string representation of a binary number to construct a bitstream from
         */
        bitstream(const std::string& t_str)
        {
            for (char c : t_str)
            {
                if (c != '0' && c != '1')
                {
                    throw std::invalid_argument("bitstream input list must be in binary");
                }

                m_bits.push_back((c == '1'));
            }
        }
        /**
         * @brief Construct a new bitstream object from c style string
         * 
         * @param t_str a string reprentation of a binary number to construct a bitstream from
         */
        bitstream(const char* t_str) : bitstream(std::string(t_str)) {}
        /**
         * @brief Construct a new bitstream object from a list of bits
         * 
         * @param t_bits a list of bits to construct a bitstream from
         */
        bitstream(const std::vector<bool>& t_bits) { m_bits = t_bits; }
        /**
         * @brief Construct a new bitstream object from a list of bits (move constructor)
         * 
         * @param t_bits a list of bits to construct a bitstream from (move)
         */
        bitstream(std::vector<bool>&& t_bits) { m_bits = std::move(t_bits); }
        /**
         * @brief Construct a new bitstream object
         * 
         * @param t_bits another bitstream to copy
         */
        bitstream(const bitstream& t_bits) { m_bits = t_bits.m_bits; }
        /**
         * @brief Construct a new bitstream object
         * 
         * @param t_bits another bitstream to move
         */
        bitstream(bitstream&& t_bits) { m_bits = std::move(t_bits.m_bits); }

        /**
         * @brief bitstream assignemnt operator
         * 
         * @param t_bits bits to copy over
         * @return bitstream& 
         */
        bitstream& operator =(const bitstream& t_bits) { m_bits = t_bits.m_bits; return (*this); }
        /**
         * @brief bitstream move assignment operator
         * 
         * @param t_bits bits to move over
         * @return bitstream& 
         */
        bitstream& operator =(bitstream&& t_bits) { m_bits = std::move(t_bits.m_bits); return (*this); }

        /**
         * @brief serializable bitstream write impl
         * 
         * @param t_stream the stream to write to
         */
        void write(std::ofstream& t_stream) const final override
        {
            serializable_list<bool> l(m_bits);
            l.write(t_stream);
        }

        /**
         * @brief seriaizable bitstream read impl
         * 
         * @param t_stream stream to read from
         */
        void read(std::ifstream& t_stream) override
        {
            serializable_list<bool> l;
            l.read(t_stream);
            m_bits = l.data();
        }

        /**
         * @brief bitstream index access operator [] (&)
         * 
         * @param t_n read from index t_n
         * @return std::vector<bool>::reference 
         */
        std::vector<bool>::reference operator [] (const int& t_n) { return m_bits[t_n]; }
        /**
         * @brief bitstream index access operator []
         * 
         * @param t_n index to read at
         * @return true if the bit is a 1
         * @return false if the bit is a 0
         */
        bool operator [] (const int& t_n) const { return m_bits[t_n]; }

        /**
         * @brief bitstream get bit at index
         * 
         * @param t_n index to get at
         * @return true if the bit at index t_n is a 1
         * @return false if the bit at index t_n is a 0
         */
        bool get(const int& t_n) const { return m_bits.at(t_n); }
        /**
         * @brief get the size of the bitstream
         * 
         * @return size_t 
         */
        size_t size() const { return m_bits.size(); }

        /**
         * @brief append a bitstream to another
         * 
         * @param t_bits other bitstream to append
         */
        void append(const bitstream& t_bits)
        { m_bits.insert(m_bits.end() , t_bits.cbegin(), t_bits.cend()); }
        /**
         * @brief append a bit to a bitstream
         * 
         * @param t_bit bit to append
         */
        void append(const bool& t_bit)
        { m_bits.push_back(t_bit); }
        /**
         * @brief prepend a bitstream
         * 
         * @param t_bits bitstream to prepend
         */
        void prepend(const bitstream& t_bits)
        { m_bits.insert(m_bits.begin() , t_bits.cbegin(), t_bits.cend()); }
        /**
         * @brief prepend a bit to a bitstream
         * 
         * @param t_bit bit to prepend
         */
        void prepend(const bool& t_bit)
        { m_bits.insert(m_bits.begin() , t_bit); }
        /**
         * @brief insert a bitstream into another
         * 
         * @param t_pos position to insert bitsream at
         * @param t_bits bitsream to insert
         */
        void insert(std::vector<bool>::iterator t_pos, const bitstream& t_bits)
        { { m_bits.insert(t_pos , t_bits.cbegin(), t_bits.cend()); } }
        /**
         * @brief insert a bit into a bitsream
         * 
         * @param t_pos position to insert at
         * @param t_bit bit to insert
         */
        void insert(std::vector<bool>::iterator t_pos, const bool& t_bit)
        { { m_bits.insert(t_pos , t_bit); } }

        /**
         * @brief apply a bitwise not to all the bits in the bitsream such that (bit) = !(bit)
         * 
         */
        void bitwise_not()
        {
            for (int i = 0; i < m_bits.size(); i++)
                m_bits[i] = !m_bits[i];
        }

        /**
         * @brief vector begin interator
         * 
         * @return std::vector<bool>::iterator 
         */
        std::vector<bool>::iterator begin() { return m_bits.begin(); }
        /**
         * @brief vector const begin iterator
         * 
         * @return std::vector<bool>::const_iterator 
         */
        std::vector<bool>::const_iterator cbegin() const { return m_bits.cbegin(); }
        /**
         * @brief vector end iterator
         * 
         * @return std::vector<bool>::iterator 
         */
        std::vector<bool>::iterator end() { return m_bits.end(); }
        /**
         * @brief vector const end iterator
         * 
         * @return std::vector<bool>::const_iterator 
         */
        std::vector<bool>::const_iterator cend() const { return m_bits.cend(); }

        /**
        * @brief get a rage of bits from the bitstream
        *
        * @param t_begin start of the new bitstream
        * @param t_end end of the new bitstream
        *
        * @return a bitstream with all the bits from t_start to t_end
        */
        bitstream get(std::vector<bool>::const_iterator t_begin, std::vector<bool>::const_iterator t_end) {
            std::vector<bool> bits(t_begin, t_end);
            return bitstream(bits);
        }

        /**
         * @brief convert a bitsream to ascii.
         * bitsream must be devisable by 8
         * 
         * @return std::string 
         */
        std::string toAscci() const
        {
            if (size() % 8 != 0)
            {
                perror("bitstream must be devisable by 8");
            }

            std::string constructedAsciiString = "";
            for (auto it = cbegin(); it != cend(); it += 8)
            {
                bitstream substr = std::vector<bool>(it, it+8);
                constructedAsciiString.push_back((char)(substr.toInt()));
            }

            return constructedAsciiString;
        }

        /**
         * @brief convert a bitstream to an ingeger
         * 
         * @return int 
         */
        int toInt() const
        {
            int count = size()-1;
            int asInt = 0;
            for (auto bit : m_bits)
            {
                asInt += (bit) ? pow(2, count) : 0;
                count--;
            }

            return asInt;
        }
        /**
         * @brief convert a bitsream to a ubint
         * 
         * @return ubint 
         */
        ubint toUbint() const
        {
            int count = size()-1;
            ubint asInt = 0ULL;
            for (auto bit : m_bits)
            {
                asInt += (bit) ? ubint(2) ^ ubint(count) : ubint();
                count--;
            }

            return asInt;
        }

        /**
         * @brief convert a bitsream to a binary string
         * 
         * @return std::string 
         */
        std::string toBinary() const
        { return std::accumulate(std::next(m_bits.begin()), m_bits.end(), std::to_string(m_bits[0]), [](std::string a, bool b) { return std::move(a) + std::to_string(b); }); }

        /**
         * @brief create a bitsream from a binary char
         * 
         * @param t_str char to construct bitstream from
         * @return bitstream 
         */
        friend bitstream fromAscii(const char &t_str);
        /**
         * @brief create a bitsream from a binary char with a given length
         * 
         * @param t_str char to construct bitsream from
         * @param t_bits fixed size of bitsream
         * @return bitstream 
         */
        friend bitstream fromAscii(const char &t_str, const int& t_bits);
        /**
         * @brief construct a bitsream from an ASCII string
         * 
         * @param t_str ASCII string to construct a bitsream from
         * @return bitstream 
         */
        friend bitstream fromAscii(const std::string &t_str);

        /**
         * @brief apply a bitwise xor between two bitsreams
         * 
         * @param t_lhs lhs of bitwise xor
         * @param t_rhs rhs of bitwise xor
         * @return bitstream 
         */
        friend bitstream bitwise_xor(const bitstream& t_lhs, const bitstream& t_rhs);
        /**
         * @brief apply a bitwise and between two bitsreams
         * 
         * @param t_lhs lhs of bitwise and
         * @param t_rhs rhs of bitwise and
         * @return bitstream 
         */
        friend bitstream bitwise_and(const bitstream& t_lhs, const bitstream& t_rhs);
        /**
         * @brief apply a bitwise or between two bitsream
         * 
         * @param t_lhs lhs of bitwise or
         * @param t_rhs rhs of bitwise or
         * @return bitstream 
         */
        friend bitstream bitwise_or(const bitstream& t_lhs, const bitstream& t_rhs);

        /**
         * @brief standard out of bitsream
         * 
         * @return std::ostream& 
         */
        friend std::ostream& operator<<(std::ostream&, const bitstream&);

    private:

        /**
         * @brief bitsream represented as a list of bools (True = 1, False = 0)
         * 
         */
        std::vector<bool> m_bits;
    };

    inline bitstream fromAscii(const char &t_str)
    {
        bitstream res;
        int num = (int)t_str;
        do
        {
            res.m_bits.push_back((bool)(num % 2));
            num /= 2;
        } while (num / 2 != 0);
        res.m_bits.push_back(num);
        std::reverse(res.begin(), res.end());

        return res;
    }

    inline bitstream fromAscii(const char &t_str, const int& t_bits)
    {
        bitstream s = fromAscii(t_str);
        s.prepend(std::vector<bool>(t_bits - s.size(), 0));
        return s;
    }
    
    inline bitstream fromAscii(const std::string &t_str)
    {
        bitstream s;
        for (char c : t_str)
        {
            s.append(fromAscii(c, 8));
        }
        return s;
    }

    inline bitstream bitwise_xor(const bitstream& t_lhs, const bitstream& t_rhs)
    { bitstream r = t_lhs; for (int i = 0; i < t_lhs.size(); i++) r[i] = t_lhs[i] ^ t_rhs[i]; return r;}
    inline bitstream bitwise_and(const bitstream& t_lhs, const bitstream& t_rhs)
    { bitstream r = t_lhs; for (int i = 0; i < t_lhs.size(); i++) r[i] = t_lhs[i] & t_rhs[i]; return r;}
    inline bitstream bitwise_or(const bitstream& t_lhs, const bitstream& t_rhs)
    { bitstream r = t_lhs; for (int i = 0; i < t_lhs.size(); i++) r[i] = t_lhs[i] | t_rhs[i]; return r;}

    inline std::ostream& operator<<(std::ostream& t_o, const bitstream& t_b)
    {
        t_o << t_b.toBinary();
        return t_o;
    }

}

#endif
