#ifndef UTIL_SERIALIZABLE_HPP
#define UTIL_SERIALIZABLE_HPP

#include <iostream>
#include <fstream>
#include <iterator>

namespace util
{
    /**
     * @brief this class will be inherited for serializable objects
     */
    class serializable
    {
    public:
        /**  
         * @brief when called, it will serialize the class into the specified file name using overloaded write method
         * 
         * @param t_filename is the name of the output filename
         */
        void serialize(std::string t_filename)
        {
            std::ofstream file(t_filename, std::ios::binary | std::ios::out);
            if (file.is_open())
            {
                file.seekp(0, std::ios::beg);
                write(file);
            }
            file.close();
        }

        /**
         * @brief deserialize method will read a serializable class from the spesified file name using overloaded read 
         * method
         * 
         * @param t_filename the specified binary file to deserialize
         */
        void deserialize(std::string t_filename)
        {
            std::ifstream file(t_filename, std::ios::binary | std::ios::in);
            if (file.is_open())
            {
                file.seekg(0, std::ios::beg);
                read(file);
            }
            file.close();
        }

        /**
         * @brief this method is overloaded by the inheriting class and specifies how to read a serialized file
         * 
         * @param t_stream the input file stream to read from
         */
        virtual void read(std::ifstream &t_stream)  = 0;

        /**
         * @brief this method is overloaded by the inheriting class and specifies how to write a serialized file
         * 
         * @param t_stream the output file stream to read from
         */
        virtual void write(std::ofstream &t_stream) const = 0;
    };

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
        std::string string()
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
    std::ostream& operator <<(std::ostream& t_out, const serializable_string& t_s)
    {
        t_out << t_s.m_data;
        return t_out;
    }

    /**
     * @brief a generic write method to write an object to a binary file
     * 
     * @tparam T the type of object to write
     * @param t_obj the object to write of type T
     * @param t_stream the binary output file stream to write to
     */
    template<typename T>
    void write(const T& t_obj, std::ofstream &t_stream)
    {
        t_stream.write(reinterpret_cast<const char*>(&t_obj), sizeof(T));
    }

    /**
     * @brief a generic read method to read an object from a binary file
     * 
     * @tparam T the type of object to read
     * @param t_obj pointer of where to read the object into
     * @param t_stream the binary input file stream to read from
     */
    template<typename T>
    void read(T* t_obj, std::ifstream &t_stream)
    {
        t_stream.read(reinterpret_cast<char*>(t_obj), sizeof(T));
    }
}

#endif