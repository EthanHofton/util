#ifndef UTIL_TREE_HPP
#define UTIL_TREE_HPP

#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <stack>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace util {

template<typename T>
class tree {
public:

    /**
    * @brief value type of the tree. MUST be default construtable
    */
    using value_type = T;
    using tree_type  = tree<value_type>;

private:

    /**
    * @brief tree_node stores a SHARED pointer (owns the next node) to the next node. also stores non-owning refrences to the previous node,
    * the node on the next level (first child), the node on the previous level (parent), the level the current node is on, and the depth the current node
    * is on aswell as the data the node stores. value_type MUST be default construcable
    */
    struct tree_node {
        std::shared_ptr<tree_node>  m_next;
        std::weak_ptr<tree_node>    m_prev;
        std::weak_ptr<tree_node>    m_in;
        std::weak_ptr<tree_node>    m_out;
        value_type                  m_data;
        int                         m_level;
        int                         m_depth;

        void insert(const value_type& t_val) {
            if (m_in.expired()) {
            }
        }
        void remove();
    };


    template<typename _T>
    class tree_iterator {
    private:

        // * give iterator access to const_iterators private fields
        template<typename _U>
        friend class tree_iterator;

        friend class tree;

        tree_iterator(const std::weak_ptr<tree_node>& t_itrn) : m_data(t_itrn) {}

    public:
        
        /**
        * @brief iterator catogory. since the tree iterator can be random access, it is a random access iterator tag
        */
        using iterator_category = std::bidirectional_iterator_tag;
        /**
        * @brief the type of the difference between two tree iterators
        */
        using difference_type   = int;
        /**
        * @brief the type of the iterator when dereferenced
        */
        using value_type        = _T;
        /**
        * @brief pointer to value type
        */
        using pointer           = value_type*;
        /**
        * @brief refrence of value type 
        */
        using reference         = value_type&;

        /**
        * @brief default tree constructor
        */
        tree_iterator() = default;
        /**
        * @brief tree iterator copy constructor
        *
        * @param t_itr the iterator to copy
        */
        tree_iterator(const tree_iterator& t_itr) : m_data(t_itr.m_data) {}

        /**
        * @brief tree iterator const promotion. const_iterator constructor from iterator
        *
        * @tparam is_const weather or not value_type is const
        * @param t_itr the non-const iterator
        */
        template<bool is_const = std::is_const_v<value_type>, typename = std::enable_if_t<is_const>>
        tree_iterator(const tree_iterator<std::remove_const_t<value_type>>& t_itr) : m_data(t_itr.m_data) {}

        reference operator*() { return (m_data.lock()->m_data); }
        pointer operator->() { return (std::addressof(m_data.lock()->m_data)); }

        tree_iterator& operator++()   { m_data = m_data.lock()->m_next; return *this; }
        tree_iterator operator++(int) { tree_iterator tmp = *this; ++(*this); return tmp; }
        tree_iterator& operator--()   { m_data = m_data.lock()->m_prev; return *this; }
        tree_iterator operator--(int) { tree_iterator tmp = *this; --(*this); return tmp; }

        friend bool operator==(const tree_iterator& t_a, const tree_iterator& t_b) { return (t_a.m_data.lock()) == (t_b.m_data.lock()); }
        friend bool operator!=(const tree_iterator& t_a, const tree_iterator& t_b) { return !(t_a == t_b); }

    private:

        std::weak_ptr<tree_node> m_data;
    };

public:

    using iterator       = tree_iterator<value_type>;
    using const_iterator = tree_iterator<const value_type>; 

    tree() = default;

    iterator       begin()        { return iterator(m_root); }
    const_iterator begin()  const { return const_iterator(m_root); }
    const_iterator cbegin() const { return const_iterator(m_root); }
    iterator       end()          { return iterator(); }
    const_iterator end()    const { return const_iterator(); }
    const_iterator cend()   const { return const_iterator(); }

private:

    std::shared_ptr<tree_node> m_root = std::make_shared<tree_node>();
};

}

#endif
