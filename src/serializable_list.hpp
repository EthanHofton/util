#ifndef UTIL_SERIALIZABLE_LIST_HPP
#define UTIL_SERIALIZABLE_LIST_HPP

#include <vector>
#include "serializable_string.hpp"

namespace util
{
    /**
     * @brief generic serializable list class
     * 
     * @tparam T the type of list
     */
    template<typename T>
    class serializable_list : public serializable
    {
    public:

        /**
         * @brief Construct a new serializable list object
         * 
         */
        serializable_list() {}
        /**
         * @brief Construct a new serializable list object from vector
         * 
         * @param t_data vector to construct list from
         */
        serializable_list(const std::vector<T>& t_data) { m_data = t_data; }
        /**
         * @brief Construct a new serializable list object from vector (move)
         * 
         * @param t_data the data to be moved
         */
        serializable_list(std::vector<T>&& t_data) { m_data = std::move(t_data); }
        /**
         * @brief Construct a new serializable list object from c list
         * 
         * @param t_data pointer to data
         * @param t_size size of data
         */
        serializable_list(T* t_data, size_t t_size) { memcpy((void*)&m_data[0], (void*)t_data, t_size); }
        /**
         * @brief Construct a new serializable list object from other serializable list
         * 
         * @param t_list list to be copied
         */
        serializable_list(const serializable_list& t_list) { m_data = t_list.m_data; }
        /**
         * @brief Construct a new serializable list object from other serializable list (move)
         * 
         * @param t_list list to be moved
         */
        serializable_list(serializable_list&& t_list) { m_data = std::move(t_list.m_data); }

        /**
         * @brief seriazable list asignment operator
         * 
         * @param t_list other list to assign to
         * @return serializable_list& 
         */
        serializable_list& operator=(const serializable_list& t_list) { m_data = t_list.m_data; return (*this); }
        /**
         * @brief serizable list assignement operator (move)
         * 
         * @param t_list other list to move data from
         * @return serializable_list& 
         */
        serializable_list& operator=(serializable_list&& t_list) { m_data = std::move(t_list.m_data); return (*this); }

        /**
         * @brief serializable read impl
         * 
         * @param t_stream the strean to read to
         */
        void read(std::ifstream &t_stream) final override
        {
            int size;
            // * write the size of the list to the stream
            util::read<int>(&size, t_stream);

            m_data.clear();
            // * write the list
            for (int i = 0; i < size; i++)
            {
                T val;
                util::read<T>(&val, t_stream);
                m_data.push_back(val);
            }
        }

        /**
         * @brief string serialization implementation. 
         * writes the strings c_str with '\0' trailing charicter
         * 
         * @param t_stream the stream to write from
         */
        void write(std::ofstream &t_stream) const final override
        {
            // * write the size of the list to the stream
            util::write<int>(m_data.size(), t_stream);
            // * write the list
            for (int i = 0; i < m_data.size(); i++)
            {
                util::write<T>(m_data[i], t_stream);
            }
        }

        /**
         * @brief returns the size of the list
         * 
         * @return size_t 
         */
        size_t size() const { return m_data.size(); }

        /**
         * @brief list index access
         * 
         * @param t_i index to acess the list at
         * @return T 
         */
        T operator[](const int& t_i) const { return m_data[t_i]; }
        /**
         * @brief list index access (ref)
         * 
         * @param t_i index to access list at
         * @return T& 
         */
        T& operator[](const int& t_i) { return m_data[t_i]; }

        /**
         * @brief std::vector list getter
         * 
         * @return std::vector<T> 
         */
        std::vector<T> data() const { return m_data; }

        /**
         * @brief serialzaible list standard out
         * 
         * @tparam T_ type of list
         * @return std::ostream& 
         */
        template<typename T_>
        friend std::ostream& operator <<(std::ostream&, const serializable_list<T_>&);

    private:

        /**
         * @brief underlying list data stored
         * 
         */
        std::vector<T> m_data;
    };

    template<typename T_>
    std::ostream& operator <<(std::ostream& t_out, const serializable_list<T_>& t_list)
    {
        t_out << "[";
        for (int i = 0; i < t_list.m_data.size() - 1; i++)
        {
            t_out << t_list[i] << ", ";
        }

        t_out << t_list.m_data.back() << "]";

        return t_out;
    }
}

#endif 