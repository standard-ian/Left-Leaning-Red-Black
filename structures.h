/*
 * Ian Leuty
 * ileuty@pdx.edu
 * 2/16/2025
 * CS302 Winter 2025
 * Program #3
 *
 ********************************************************************
 *
 * data structures declaration
 * left leaning red black tree and node
 *
 * inspired by the algorithm documented here:
 *      https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
 *
 *********************************************************************
 */
#ifndef RB_TREE
#define RB_TREE

#include <optional>
#include <utility>
#include <cassert>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>

//exceptions related to the red black tree
struct TREE_ERROR
{
        struct duplicate_name_exception{
            std::string msg{"\nContestant already registered.\n"};
        };

        struct not_found_exception{
            std::string msg{"\nContestant not registered.\n"};
        };
};

//red or black node
enum class Color{RED, BLACK};

template<typename KEY, typename DATA>
class Node
{
    public:
        Node(KEY key_in, DATA data_in, Color color_in);
        static bool is_red(const Node *node);
        std::string to_string() const;
        void to_string(std::stringstream &ss, const std::string &prefix, const std::string &child_prefix) const;

    private:
        KEY key;
        DATA data;
        Color color;
        std::unique_ptr<Node> left, right;
        int indentation_lvl;

    //friend of the tree so no getters/setters
    //but private is still private
    template <typename K, typename D> friend class Red_Black;
};

//red black tree interface
template<typename KEY, typename DATA>
class Red_Black
{
    typedef Node<KEY, DATA> rb_node;
    typedef std::unique_ptr<rb_node> node_ptr;

    public:
        Red_Black();
        Red_Black(const Red_Black &source);
        Red_Black<KEY, DATA>& operator=(const Red_Black &source);
        std::string tree_string() const;

        int display();
        int size() const;
        bool insert(const KEY &key, const DATA &data);
        bool find(const KEY &key) const;
        DATA& retrieve(const KEY &key);
        int fetch_keys(std::vector<KEY> &keys) const;
        int fetch_data(std::vector<DATA> &data);
        bool remove(const KEY &key);
        int remove_all();

    private:
        node_ptr root;

        //check and change colors
        bool is_red(const rb_node *node);
        void flip_colors(rb_node *source);

        //used to build accurate string representation
        int indentation(const rb_node *root) const;

        //rotations and moves
        node_ptr rotate_left(node_ptr &node);
        node_ptr red_left(node_ptr &node);
        node_ptr rotate_right(node_ptr &node);
        node_ptr red_right(node_ptr &node);

        //public method helpers
        int display(const rb_node *root);
        node_ptr insert(node_ptr &root, const KEY &key, const DATA &data);
        int fetch_keys(const rb_node *root, std::vector<KEY> &keys) const;
        int fetch_data(const rb_node *root, std::vector<DATA> &data);
        void make_copy(const node_ptr &source, node_ptr &dest);
        int size(const rb_node *root) const;

        //removal functions
        node_ptr remove(node_ptr &root, const KEY &key);
        KEY& retrieve_ios_key(rb_node *root);
        DATA& retrieve_ios_data(rb_node *root);
        node_ptr remove_ios(node_ptr &root);
        node_ptr fixup(node_ptr &root);
};

//helpers to avoid dereferencing a nullptr
//while avoiding complicated specialization
//for non-pointer data
template<typename D>
void display_data(const D& data)
{
    std::cout << data;
}
//for shared_ptr data
template<typename D>
void display_data(const std::shared_ptr<D> &data)
{
    std::cout << *data.get();
}

//overloaded ostream operator for the tree
//prints graphical representation of the keys when called
template<typename KEY, typename DATA>
std::ostream &operator<<(std::ostream &out, const Red_Black<KEY, DATA> &rb_tree)
{
    return out << "\nThe Tree:\n\n" << rb_tree.tree_string() << std::endl;
}

#include "structures.tpp"

#endif

