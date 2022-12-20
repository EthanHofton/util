#ifndef UTIL_TREE_HPP
#define UTIL_TREE_HPP

#include <cstddef>
#include <iostream>
#include <iterator>
#include <memory>
#include <stack>
#include <vector>

namespace util {

template<typename T>
class tree {
protected:
    struct node;
    struct cursor;

public:

    using tree_type = T;


    struct iterator {
        friend class tree<tree_type>;

        using iterator_category = std::forward_iterator_tag;
        using difference_type   = int;
        using value_type        = tree_type;
        using pointer           = tree_type*;
        using reference         = tree_type&;

        iterator(node* t_ptr = nullptr) : m_cursor(cursor(t_ptr)) {}

        reference operator*() { return m_cursor.m_node->m_data; }
        pointer operator->() { return std::addressof(m_cursor.m_node->m_data); }

        // * implament move
        iterator& operator++() { 
            preorder_walk(m_cursor);
            return *this;
        }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const iterator& t_a, const iterator& t_b) { return t_a.m_cursor.m_node == t_b.m_cursor.m_node; }
        friend bool operator!=(const iterator& t_a, const iterator& t_b) { return !(t_a== t_b); }

        iterator parent() { return iterator(m_cursor.m_node->m_parent); }
        iterator next_sibling() { return iterator(m_cursor.m_node->m_next); }
        iterator first_child() { return iterator((m_cursor.m_node->m_children.empty) ? nullptr : m_cursor.m_node->m_children.front()); }

    private:
        
        cursor m_cursor;
    };

    struct const_iterator {
        friend class tree<tree_type>;

        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = const tree_type;
        using pointer           = const tree_type*;
        using reference         = const tree_type&;

        const_iterator(node* t_ptr = nullptr) : m_cursor(cursor(t_ptr)) {}
        const_iterator(const iterator& t_itr) : m_cursor(cursor(t_itr.m_cursor)) {}

        reference operator*() const { return m_cursor.m_node->m_data; }
        pointer operator->() const { return std::addressof(m_cursor.m_node->m_data); }

        // * implament move
        const_iterator& operator++() { 
            preorder_walk(m_cursor);
            return *(this);
        }
        const_iterator  operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

        friend bool operator==(const const_iterator& t_a, const const_iterator& t_b) { return t_a.m_cursor.m_node == t_b.m_cursor.m_node; }
        friend bool operator!=(const const_iterator& t_a, const const_iterator& t_b) { return !(t_a== t_b); }

        const_iterator parent() { return iterator(m_cursor.m_node->m_parent); }
        const_iterator next_sibling() { return iterator(m_cursor.m_node->m_next); }
        const_iterator first_child() { return iterator((m_cursor.m_node->m_children.empty) ? nullptr : m_cursor.m_node->m_children.front()); }

    private:
        
        cursor m_cursor;
    };

    tree() { m_root = new node; }
    tree(const tree_type& t_data) { m_root = new node; m_root->m_data = t_data; }

    iterator operator[](const tree_type& t_val) { return find(t_val); }

    // * iterators *
    iterator begin() { return iterator(m_root); }
    const_iterator begin() const { return const_iterator(m_root); }
    const_iterator cbegin() const { return const_iterator(m_root); }
    iterator end() { return iterator(nullptr); }
    const_iterator end() const { return const_iterator(nullptr); }
    const_iterator cend() const { return const_iterator(nullptr); }


    // * tree operations *
    iterator add(const_iterator t_node, const tree_type& t_val) {
        node* parent = t_node.m_cursor.m_node;
        node* new_node = new node;
        node* prev_sibling = (parent->m_children.empty()) ? nullptr : parent->m_children.back();
        if (prev_sibling != nullptr) { prev_sibling->m_next = new_node; }
        if (parent != nullptr) { parent->m_children.push_back(new_node); }
        new_node->m_data = t_val;
        new_node->m_parent = parent; 
        m_size++;
        return iterator(new_node);
    }
    const_iterator cadd(const_iterator t_node, const tree_type& t_val) {
        return const_iterator(add(t_node, t_val));
    }
    // * TODO:
    void add(const_iterator t_node, const_iterator t_start, const_iterator t_end);
    // * TODO:
    void add(const_iterator t_node, const tree<T>& t_tree);
    // * TODO:
    T remove(const_iterator& t_pos);

    // * member access *
    iterator find(const tree_type& t_val) {
        iterator it;
        for (it = begin(); it != end(); it++) {
            if (*it == t_val) {
                break;
            }
        }
        return it;
    }
    const_iterator cfind(const tree_type& t_val) const {
        return const_iterator(find(t_val));
    }
    iterator find_occurance(const tree_type& t_val, const int& t_occurance) {
        iterator it;
        int occurances = 0;
        for (it = begin(); it != end(); it++) {
            if (*it == t_val && occurances == t_occurance - 1) {
                break;
            }
        }

        return it;
    }
    const_iterator cfind_occurance(const tree_type&t_val, const int& t_occurance) {
        return const_iterator(find_occurance(t_val, t_occurance));
    }

    // * tree manipulations *
    // TODO:
    tree subtree(const_iterator t_start) const;

    // * data access *
    void clear() { m_root = nullptr; m_size = 0; }
    inline int size() const { return m_size; }
    inline int size_s() const { int count = 0; for (const_iterator it = begin(); it != end(); it++, count++) {} return count;}
    bool empty() const { return m_root == nullptr; }

    // * std out *
    template<typename OT>
    friend std::ostream& operator<<(std::ostream&, const util::tree<OT>&);

protected:

    struct cursor {
        cursor(node* t_node) : m_node(t_node) {}
        bool tryMoveFirstChild() { if (m_node->m_children.empty()) { return false; } m_node = m_node->m_children.front(); return true; }
        bool tryMoveNextSibling() { if (m_node->m_next == nullptr) { return false; } m_node = m_node->m_next; return true; }
        bool moveParent() { m_node = m_node->m_parent; return (m_node != nullptr); }

        node* m_node;
    };

    struct node {
        tree_type m_data;
        node* m_parent = nullptr;
        node* m_next = nullptr;
        std::vector<node*> m_children;
    };

    static void preorder_walk(cursor& t_cursor) {
        if (t_cursor.tryMoveFirstChild()) {
            return;
        }

        do {
            if (t_cursor.tryMoveNextSibling()) {
                return;
            }
        } while (t_cursor.moveParent());

        return;
    }

    node* m_root;
    int m_size;

private:

    void pretty_print(std::ostream& t_out, const std::string& t_prefix, const node* t_node, bool t_isLeft) const {
        if (t_node != nullptr) {
            t_out << t_prefix;
            t_out << (t_isLeft ? "├──" : "└──");

            t_out << t_node->m_data << std::endl; 

            for (node* node : t_node->m_children) {
                pretty_print(t_out, t_prefix + (t_isLeft ? "│   " : "    "), node, (node != t_node->m_children.back()));
            }
        }
    }
    
};

template<typename OT>
inline std::ostream& operator<<(std::ostream& t_out, const util::tree<OT>& t_tree) {
    t_tree.pretty_print(t_out, "", t_tree.m_root, false);

    return t_out;
}

}

#endif
