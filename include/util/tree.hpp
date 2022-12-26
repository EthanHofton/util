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
    /**
    * @brief forward declate node class
    */
    struct node;
    /**
    * @brief forward declare cursor class
    */
    struct cursor;

public:

    /**
    * @brief tree_type typedef
    */
    using tree_type = T;


    /**
    * @brief iterator class. used to traverse through the tree
    */
    struct iterator {
        /**
        * @brief declare tree as friend
        */
        friend class tree<tree_type>;

        /**
        * @brief iterator catagoray
        */
        using iterator_category = std::forward_iterator_tag;
        /**
        * @brief difference between two iterators (int)
        */
        using difference_type   = int;
        /**
        * @brief the value type of the iterator
        */
        using value_type        = tree_type;
        /**
        * @brief a pointer to the value type
        */
        using pointer           = tree_type*;
        /**
        * @brief a refernce to the value type
        */
        using reference         = tree_type&;

        /**
        * @brief iterator constructor. takes a weak pointer (reference count not changed) to a node
        *
        * @param t_ptr weak pointer to the node we are iterating from
        */
        iterator(std::weak_ptr<node> t_ptr = std::weak_ptr<node>()) : m_cursor(cursor(t_ptr)) {}

        /**
        * @brief derefence operator
        *
        * @return a reference to the data the node stores
        */
        reference operator*() { return m_cursor.m_node.lock()->m_data; }
        /**
        * @brief arrow operator
        *
        * @return return a pointer to the data the node stores
        */
        pointer operator->() { return std::addressof(m_cursor.m_node.lock()->m_data); }

        /**
        * @brief iterator increment operator. step one node (preorder) in the tree
        *
        * @return a refrence to itself
        */
        iterator& operator++() { 
            preorder_walk(m_cursor);
            return *this;
        }
        /**
        * @brief increment operator postfix. (increment after return)
        *
        * @param int the ammount to increment by
        *
        * @return the unchanged iterator
        */
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        /**
        * @brief equality operator
        *
        * @param t_a first iterator to compare
        * @param t_b second iterator to compare
        *
        * @return if the node the two iterators are pointing to is the same
        */
        friend bool operator==(const iterator& t_a, const iterator& t_b) { return t_a.m_cursor.m_node.lock() == t_b.m_cursor.m_node.lock(); }
        /**
        * @brief inequality operator
        *
        * @param t_a the first iterator to conmpare
        * @param t_b the second iterator to copmare
        *
        * @return if the node the two iterators are pointing to is not equal 
        */
        friend bool operator!=(const iterator& t_a, const iterator& t_b) { return !(t_a== t_b); }

        /**
        * @brief get an iterator to the parent the current iterator is pointing to
        *
        * @return parent iterator
        */
        iterator parent() { return iterator(m_cursor.m_node.lock()->m_parent); }
        /**
        * @brief get an iterator to the next sibling of the current iterator
        *
        * @return sibling iterator
        */
        iterator next_sibling() { return iterator(m_cursor.m_node.lock()->m_next); }
        /**
        * @brief get an iterator the the first child of the current iterator
        *
        * @return first child iterator
        */
        iterator first_child() { return iterator((m_cursor.m_node.lock()->m_children.empty) ? nullptr : m_cursor.m_node->m_children.front()); }

    private:
        
        cursor m_cursor;
    };

    /**
    * @brief const impl of iterator
    */
    struct const_iterator {
        /**
        * @brief declare tree as friend class
        */
        friend class tree<tree_type>;

        /**
        * @brief iterator catagory (forwad)
        */
        using iterator_category = std::forward_iterator_tag;
        /**
        * @brief iterator difference type (int)
        */
        using difference_type   = std::ptrdiff_t;
        /**
        * @brief iterator value_type (what dereferenceing the iterator returns a refrence to)
        */
        using value_type        = const tree_type;
        /**
        * @brief iterator value type pointer
        */
        using pointer           = const tree_type*;
        /**
        * @brief iterator reference type
        */
        using reference         = const tree_type&;

        /**
        * @brief const iterator constructor from a weak pointer. create a new cursor with the weak pointer
        *
        * @param t_ptr the pointer to create an iterator from
        */
        const_iterator(std::weak_ptr<node> t_ptr = std::weak_ptr<node>()) : m_cursor(cursor(t_ptr)) {}
        /**
        * @brief const iterator regular iterator consturcotr. construct a const_iterator from an iterator
        *
        * @param t_itr the iterator to construct a const iterator from
        */
        const_iterator(const iterator& t_itr) : m_cursor(cursor(t_itr.m_cursor)) {}

        /**
        * @brief iterator derefence operator
        *
        * @return a cosnt refrence to the data the iterator holds
        */
        reference operator*() const { return m_cursor.m_node.lock()->m_data; }
        /**
        * @brief iterator arrow operator
        *
        * @return a const pointer to the data the iterator holds
        */
        pointer operator->() const { return std::addressof(m_cursor.m_node.lock()->m_data); }

        /**
        * @brief iterator increment operator. move 1 step (prefix) down the stree
        *
        * @return a refrence to itself, walked one step down the tree
        */
        const_iterator& operator++() { 
            preorder_walk(m_cursor);
            return *(this);
        }
        /**
        * @brief iterator postfix increment 
        *
        * @param int the amouint to increment
        *
        * @return const iterator unchanged
        */
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }

        /**
        * @brief const iterator equality check
        *
        * @param t_a first iterator to compare
        * @param t_b second iterator to compare
        *
        * @return if the node `t_a` is the same as `t_b` is pointing to
        */
        friend bool operator==(const const_iterator& t_a, const const_iterator& t_b) { return t_a.m_cursor.m_node.lock() == t_b.m_cursor.m_node.lock(); }
        /**
        * @brief const iterator inequality check
        *
        * @param t_a first iterator to compare
        * @param t_b second iterator to compare
        *
        * @return if the node `t_a` is pointing to is not the same as `t_b`
        */
        friend bool operator!=(const const_iterator& t_a, const const_iterator& t_b) { return !(t_a== t_b); }

        /**
        * @brief get the parent iterator of the current iterator
        *
        * @return parent node iterator
        */
        const_iterator parent() { return iterator(m_cursor.m_node.lock()->m_parent); }
        /**
        * @brief get the next sibling iterator of the current iterator
        *
        * @return sibling node iteraotr 
        */
        const_iterator next_sibling() { return iterator(m_cursor.m_node.lock()->m_next); }
        /**
        * @brief get the first child iterator of the current iterator
        *
        * @return first child iterator
        */
        const_iterator first_child() { return iterator((m_cursor.m_node.lock()->m_children.empty) ? nullptr : m_cursor.m_node.lock()->m_children.front()); }

    private:
        
        cursor m_cursor;
    };

    // * destrctor and constructor

    /**
    * @brief tree constructor. intialize the root node
    */
    tree() { m_root = std::make_shared<node>(); }
    /**
    * @brief tree constructor with data. intialize the root node and set the nodes data
    *
    * @param t_data the data to intialize the root node with
    */
    tree(const tree_type& t_data) { m_root = std::make_shared<node>(); m_root->m_data = t_data; }
    /**
    * @brief tree copy consturctor. create a copy of the shared root node pointer
    *
    * @param t_other the other tree to link to
    */
    tree(const tree<tree_type>& t_other) { m_root = t_other.m_root; m_size = t_other.m_size; }
    /**
    * @brief tree move constuctor. move the root node to this trees root node
    *
    * @param t_other the other tree to move
    */
    tree(tree&& t_other) { m_root = std::move(t_other.m_root); m_size = t_other.m_size; }
    /**
    * @brief tree discturor. release the root node
    */
    ~tree() { clear(); }

    // * operators *

    /**
    * @brief tree memeber access. returns an iterator to the first occurance of the input. tree::end is returned if not found
    *
    * @param t_val value to find in tree
    *
    * @return iterator to the node containing that value (or end)
    */
    iterator operator[](const tree_type& t_val) { return find(t_val); }
    /**
    * @brief tree assignment operator
    *
    * @param t_other the other tree to link to
    *
    * @return refreunce to self
    */
    tree& operator=(const tree<tree_type>& t_other) { m_root = t_other.m_root; m_size = t_other.m_size; return *this; }
    /**
    * @brief tree assignemtn move. moves root node pointer to this tree
    *
    * @param t_other other tree to move
    *
    * @return refrence to self
    */
    tree& operator=(tree<tree_type>&& t_other) { m_root = std::move(t_other.m_root); m_size = t_other.m_size; return *this; }

    // * iterators *

    /**
    * @brief iterator to the beginning of tree (root node)
    *
    * @return iterator to begining
    */
    iterator begin() { return iterator(m_root); }
    /**
    * @brief const_iterator to beginning of tree (root node)
    *
    * @return const_iterator to beginning
    */
    const_iterator begin() const { return const_iterator(m_root); }
    /**
    * @brief const_iterator to begininng of tree
    *
    * @return const_iterator to beginning
    */
    const_iterator cbegin() const { return const_iterator(m_root); }
    /**
    * @brief iterator to end of tree (nullptr)
    *
    * @return iterator to end
    */
    iterator end() { return iterator(); }
    /**
    * @brief const iterator to end of tree (nullptr)
    *
    * @return const iterator to end of tree
    */
    const_iterator end() const { return const_iterator(); }
    /**
    * @brief const iterator to end of tree (nullptr)
    *
    * @return const iterator to end
    */
    const_iterator cend() const { return const_iterator(); }


    // * tree operations *

    /**
    * @brief add a new child to node at iterator `t_node`
    *
    * @param t_node the node to add to
    * @param t_val the value to add to that nodes children
    *
    * @return an iterator to the node just added
    */
    iterator add(const_iterator t_node, const tree_type& t_val) {
        std::weak_ptr<node> parent = t_node.m_cursor.m_node;
        std::shared_ptr<node> new_node = std::make_shared<node>();
        if (!parent.expired() && !parent.lock()->m_children.empty()) { parent.lock()->m_children.back()->m_next = new_node; }
        if (!parent.expired()) { parent.lock()->m_children.push_back(new_node); }
        new_node->m_data = t_val;
        new_node->m_parent = parent; 
        m_size++;
        return iterator(new_node);
    }
    /**
    * @brief add a new child to node at iterator `t_node`
    *
    * @param t_node node to add to
    * @param t_val value to create new node with
    *
    * @return const iterator to new node
    */
    const_iterator cadd(const_iterator t_node, const tree_type& t_val) {
        return const_iterator(add(t_node, t_val));
    }
    /**
    * @brief add a subtree to a node
    *
    * @param t_node the node to add the subtree to
    * @param t_tree the subtree to add to the node
    */
    void add(const_iterator t_node, const tree<T>& t_tree);
    /**
    * @brief remove a node
    *
    * @param t_pos the node to remove
    *
    * @return the value that was just removed
    */
    tree_type remove(const_iterator& t_pos);

    // * member access *

    /**
    * @brief find a node in the tree
    *
    * @param t_val the value to find
    *
    * @return iterator pointing to that node
    */
    iterator find(const tree_type& t_val) {
        iterator it;
        for (it = begin(); it != end(); it++) {
            if (*it == t_val) {
                break;
            }
        }
        return it;
    }
    /**
    * @brief find a ndoe in the tree
    *
    * @param t_val the value to find
    *
    * @return const iterator pointing to that node
    */
    const_iterator cfind(const tree_type& t_val) const {
        return const_iterator(find(t_val));
    }
    /**
    * @brief find nth occurance in a tree
    *
    * @param t_val the val to find
    * @param t_occurance the nth occurance to find
    *
    * @return a pointer to the found node
    */
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
    /**
    * @brief find nth occurance in a tree
    *
    * @param t_val the value to find
    * @param t_occurance the nth occurance to find
    *
    * @return a const iterator pointing to the found node
    */
    const_iterator cfind_occurance(const tree_type&t_val, const int& t_occurance) {
        return const_iterator(find_occurance(t_val, t_occurance));
    }

    // * tree manipulations *

    /**
    * @brief get a subtree from a node
    *
    * @param t_start the node to get the subtree from
    *
    * @return the subtree at node `t_start`
    */
    tree subtree(const_iterator t_start) const {
        return util::tree<tree_type>(t_start.m_cursor.m_node.lock());
    }

    // * data access *

    /**
    * @brief clear all the elements in a tree 
    */
    void clear() { m_root = nullptr; m_size = 0; }
    /**
    * @brief get the size of the tree
    *
    * @return the size of the tree (cached)
    */
    inline int size() const { return m_size; }
    /**
    * @brief calculate the size of a tree (size_slow)
    *
    * @return the size of the tree
    */
    inline int size_s() const { int count = 0; for (const_iterator it = begin(); it != end(); it++) { count++; } return count;}
    /**
    * @brief check if the tree containts any data
    *
    * @return if the tree is empty
    */
    bool empty() const { return m_root == nullptr; }

    // * std out *

    /**
    * @brief std::cout
    *
    * @tparam OT tree_type
    * @param std::ostream output stream to print to
    * @param util::tree tree to print
    *
    * @return ostream refreance
    */
    template<typename OT>
    friend std::ostream& operator<<(std::ostream&, const util::tree<OT>&);


    // * tree copying *
    /**
    * @brief return a copy of the tree
    *
    * @return a copy of the tree
    */
    tree<tree_type> copy() {
        iterator i = begin();
        tree<tree_type> c = *i;
        iterator ci = c.begin();

        bool looping = true;

        while (ci != end()) {
            if (i.m_cursor.tryMoveFirstChild()) {
                c.add(ci, *i);
                ci.m_cursor.tryMoveFirstChild();
                continue;
            }

            do {
                if (i.m_cursor.tryMoveNextSibling()) {
                    c.add(ci.parent(), *i);
                    ci.m_cursor.tryMoveNextSibling();
                    break;
                }

                looping = ci.m_cursor.moveParent();
                i.m_cursor.moveParent();
            } while (looping);
        }

        return c;
    }

protected:

    /**
    * @brief cursor class. stores where the cursor is in in the tree and can move arround
    */
    struct cursor {
        /**
        * @brief cursor constructor. stores the start node
        *
        * @param t_node the node to start the cursor from
        */
        cursor(std::weak_ptr<node> t_node) : m_node(t_node) {}
        /**
        * @brief try to move to the nodes first child. If no child exists, no move is done and false is returned. otherwise reutrn true
        *
        * @return if the cursor moved to the child
        */
        bool tryMoveFirstChild() { if (m_node.lock()->m_children.empty()) { return false; } m_node = m_node.lock()->m_children.front(); return true; }
        /**
        * @brief try to move to the next sibling of the node. if no sibling exists, no move is done and false is returned. otherwise treu
        *
        * @return if the cursor moved to the next subing
        */
        bool tryMoveNextSibling() { if (m_node.lock()->m_next.expired()) { return false; } m_node = m_node.lock()->m_next; return true; }
        /**
        * @brief move the cursor to parent
        *
        * @return if the parent is not null
        */
        bool moveParent() { m_node = m_node.lock()->m_parent; return (!m_node.expired()); }

        /**
        * @brief the node the cursor is at
        */
        std::weak_ptr<node> m_node;
    };

    /**
    * @brief all data relating to each tree node
    */
    struct node {
        /**
        * @brief the data the node is storing
        */
        tree_type m_data;
        /**
        * @brief weak pointer to parent
        */
        std::weak_ptr<node> m_parent;
        /**
        * @brief weak pointer to next sibling
        */
        std::weak_ptr<node> m_next;
        /**
        * @brief list of shared pointers to all nodes children
        */
        std::vector<std::shared_ptr<node>> m_children;
    };

    /**
    * @brief walk the cursor to next node in tree. (preorder)
    *
    * @param t_cursor the cursor to walk
    */
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

    /**
    * @brief shared pointer to root node
    */
    std::shared_ptr<node> m_root;
    /**
    * @brief size of the tree
    */
    int m_size = 0;

private:

    tree(std::shared_ptr<node> t_startNode) { m_root = t_startNode; m_size = 0; }

    void pretty_print(std::ostream& t_out, const std::string& t_prefix, const std::weak_ptr<node> t_node, bool t_isLeft) const {
        if (!t_node.expired()) {
            t_out << t_prefix;
            t_out << (t_isLeft ? "├──" : "└──");

            t_out << t_node.lock()->m_data << std::endl; 

            for (std::weak_ptr<node> node : t_node.lock()->m_children) {
                pretty_print(t_out, t_prefix + (t_isLeft ? "│   " : "    "), node, (node.lock() != t_node.lock()->m_children.back()));
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
