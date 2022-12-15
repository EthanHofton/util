#ifndef UTIL_SERIALIZABLE_STRING_HPP
#define UTIL_SERIALIZABLE_STRING_HPP

#include "serializable.hpp"

namespace util
{
    /**
     * @brief serializable string impl
     */
    class serializable_string : public serializable
    {
    public:

        /**
         * @brief Construct a new serializable string object
         */
        serializable_string() {}
        /**
         * @brief Construct a new serializable string object
         * 
         * @param t_data char * data to create string from
         */
        serializable_string(const char* t_data) { m_data = std::string(t_data); }
        /**
         * @brief Construct a new serializable string object
         * 
         * @param t_data string data for the seriazliable string
         */
        serializable_string(const std::string& t_data) { m_data = t_data; }
        /**
         * @brief Construct a new serializable string object (string move constructor)
         * 
         * @param t_data rvalue to string to construct string from
         */
        serializable_string(std::string&& t_data) { m_data = std::move(t_data); }
        /**
         * @brief Construct a new serializable string object
         * 
         * @param t_data serializable_string to construct new serializable_string from
         */
        serializable_string(const serializable_string& t_data) { m_data = t_data.m_data; }
        /**
         * @brief Construct a new serializable string object (serializable_string move constructor)
         * 
         * @param t_data rvalue to serializable_string to construct string from
         */
        serializable_string(serializable_string&& t_data) { m_data = std::move(t_data.m_data); }

        /**
         * @brief serializable_string assignement operator
         * 
         * @param t_other the other string to copy
         * @return serializable_string& 
         */
        serializable_string& operator =(const std::string& t_other) { m_data = t_other; return (*this); }
        /**
         * @brief serializable_string assignement operator (string move)
         * 
         * @param t_other the other string to move
         * @return serializable_string& 
         */
        serializable_string& operator =(std::string&& t_other) { m_data = std::move(t_other); return (*this); }
        /**
         * @brief serializable_string assignement operator
         * 
         * @param t_other the other const char* to copy
         * @return serializable_string& 
         */
        serializable_string& operator =(const char* t_other) { m_data = std::string(t_other); return (*this); }
        /**
         * @brief serializable_string assignement operator
         * 
         * @param t_other the other serializable string to copy
         * @return serializable_string& 
         */
        serializable_string& operator =(const serializable_string& t_other) { m_data = t_other.m_data; return (*this); }
        /**
         * @brief serializable_string assignement operator (serializable stirng move)
         * 
         * @param t_other the other serializable string to move
         * @return serializable_string& 
         */
        serializable_string& operator =(serializable_string&& t_other) { m_data = std::move(t_other.m_data); return (*this); }

        /**
         * @brief string deserialization implementation. 
         * repeatedly reads from stream until it finds '\0' string terminator
         * 
         * @param t_stream the stream to read from
         */
        void read(std::ifstream &t_stream) final override
        {
            std::istreambuf_iterator<char> eos;
            std::istreambuf_iterator<char> it(t_stream);

            while (it!=eos && *it!='\0') m_data += *it++;
            it++;
        }

        /**
         * @brief string serialization implementation. 
         * writes the strings c_str with '\0' trailing charicter
         * 
         * @param t_stream the stream to write from
         */
        void write(std::ofstream &t_stream) const final override
        {
            t_stream.write(m_data.c_str(), m_data.size() + 1);
        }

        /**
         * @brief get the value of the stored string
         * 
         * @return std::string 
         */
        std::string string() const
        {
            return m_data;
        }

        /**
         * @brief print serializable string
         * 
         * @return std::ostream& 
         */
        friend std::ostream& operator <<(std::ostream&, const serializable_string&);

    private:

        /**
         * @brief store the string data
         * 
         */
        std::string m_data;
    };

    /**
     * @brief push serializable string to ostream 
     * 
     * @param t_out ostream to push to
     * @param t_s serializable string to print
     * @return std::ostream& 
     */
    inline std::ostream& operator <<(std::ostream& t_out, const serializable_string& t_s)
    {
        t_out << t_s.m_data;
        return t_out;
    }

    /**
     * @brief write serializable string specilisation
     * 
     * @tparam  
     * @param t_obj serizable list to write
     * @param t_stream stream to write to
     */
    template<>
    inline void write<serializable_string>(const serializable_string& t_obj, std::ofstream &t_stream)
    {
        t_obj.write(t_stream);
    }

    /**
     * @brief read serializable string specilization
     * 
     * @tparam  
     * @param t_obj serializable string object to read to
     * @param t_stream stream to read from
     */
    template<>
    inline void read<serializable_string>(serializable_string* t_obj, std::ifstream &t_stream)
    {
        t_obj->read(t_stream);
    }

    /**
     * @brief write std::string specialization
     * 
     * @tparam  
     * @param t_obj std::string object to write
     * @param t_stream stream to write to
     */
    template<>
    inline void write<std::string>(const std::string& t_obj, std::ofstream &t_stream)
    {
        serializable_string s = t_obj;
        s.write(t_stream);
    }

    /**
     * @brief read std::string specalization
     * 
     * @tparam  
     * @param t_obj std::string object to read to
     * @param t_stream stream to read from
     */
    template<>
    inline void read<std::string>(std::string* t_obj, std::ifstream &t_stream)
    {
        serializable_string s;
        s.read(t_stream);
        *t_obj = s.string();
    }
}

#endif
