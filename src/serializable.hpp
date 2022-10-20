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

        virtual ~serializable() {}

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