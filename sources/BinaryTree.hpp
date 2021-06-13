#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>

namespace ariel
{
    enum class OrderType{
        inorder,
        preorder,
        postorder
    };
    template <typename T>
    class BinaryTree
    {
    private:
        struct Node
        {
            T value;
            Node *right_child;
            Node *left_child;

            explicit Node(T val, Node *r = nullptr, Node *l = nullptr) : value(val), right_child(r), left_child(l){};

            // copy constructor for node
           /* Node(const Node& rhs): value(rhs.value)
            {
                if (rhs.right_child != nullptr) {
                    right_child = new Node(*(rhs.right_child));
                }
                if (rhs.left_child != nullptr) {
                    left_child = new Node(*(rhs.left_child));
                }
            }*/

        };

        void free_memory(Node *node) {
            if (node == nullptr) {
                return;
            }
            free_memory(node->left_child);
            free_memory(node->right_child);
            delete node;
        }

        void tree_copy(Node *store_where, Node *other)
        {
            if (other->left_child != nullptr)
            {
                store_where->left_child = new Node(other->left_child->value);
                tree_copy( store_where->left_child ,other->left_child );
            }
            if (other->right_child != nullptr)
            {
                store_where->right_child = new Node(other->right_child->value);
                tree_copy( store_where->right_child ,other->right_child );
            }
        }

        void recursive_inorder(Node *next)
        {
            if (next == nullptr)
            {
                return;
            }
            recursive_inorder(next->left_child);
            in.push_back(next);
            recursive_inorder(next->right_child);
        }

        void recursive_preorder(Node *next)
        {
            if (next == nullptr)
            {
                return;
            }
            pre.push_back(next);
            recursive_preorder(next->left_child);
            recursive_preorder(next->right_child);
        }

        void recursive_postorder(Node *next)
        {
            if (next == nullptr)
            {
                return;
            }
            recursive_postorder(next->left_child);
            recursive_postorder(next->right_child);
            post.push_back(next);
        }

        Node *root;
        std::vector< Node * > in  , pre , post;
        std::vector< Node *>&  get_order(const OrderType type){
            switch (type) {
                case OrderType::inorder:
                {
                    return in;
                }
                case OrderType::preorder:
                {
                    return pre;
                }
                case OrderType::postorder:
                {
                    return post;
                }
            }

        }
    public:
        void debug(){
            in.clear();
            pre.clear();
            post.clear();
            recursive_inorder(root);
            recursive_preorder(root);
            recursive_postorder(root);
            for (auto n:in) {
                std:: cout << n->value << " ";
            }
            std::cout << std::endl;
            for (auto n:pre) {
                std:: cout << n->value << " ";
            }
            std::cout << std::endl;
            for (auto n:post) {
                std:: cout << n->value << " ";
            }
            std::cout << std::endl;
        }
        BinaryTree():root(nullptr) , in() , pre() , post(){}

        BinaryTree(BinaryTree &other) //ctor constructor
        {
            this->root = new Node(other.root->value);
            tree_copy(this->root , other.root);
        }

        ~BinaryTree()
        {
            free_memory(this->root);
        }

        // move constructor (moves from existing obj to non existing obj)
        BinaryTree(BinaryTree<T> &&other) noexcept
        {
            this->root = other.root;
            other.root = nullptr;
        }

        // move assignment (from existing obj to existing obj)
        BinaryTree &operator=(BinaryTree<T> &&bt) noexcept
        {
            if (this->root)
            {
                free_memory(this->root);
            }
            this->root = bt.root;
            bt.root = nullptr;
            return *this;
        }

        // deep copy
        BinaryTree<T> &operator=(const BinaryTree<T> &copy_from)
        {
            if (this != &copy_from)
            {
                Node * temp = new Node(copy_from.root->value);
                tree_copy(temp,copy_from.root);
                free_memory(this->root);
                root = temp;
            }
            return *this;
        }

        BinaryTree<T> &add_root(T make_root)
        {
            if (this->root == nullptr)
            {
                this->root = new Node(make_root);
            }
            else
            {
                this->root->value = make_root;
            }
            return *this;
        }

        // TODO actualize ostream operator
        template<typename E>
        friend std::ostream &operator<<(std::ostream &os, BinaryTree<E> &print)
        {
            std::cout << "inorder presentation" << endl;
            for (auto stored : in)
            {
                cout <<
            }
            return os;
        }*/


        class iterator
        {
            BinaryTree* tree;
            OrderType type;
            unsigned int location;

        public:
            explicit iterator(BinaryTree* tree, OrderType iter_type , unsigned int location = 0):
            tree(tree), type(iter_type), location(location){};

            Node *get_curr_node() const
            {
                return ((*this->tree).get_order(this->type)).at(this->location);
            }

            iterator &operator++() // ++i
            {
                location++;
                return *this;
            }

            iterator operator++(int) // i++
            {
                iterator tmp = *this;
                ++location;
                return tmp;
            }

//            iterator &operator=(T *other)
//            {
//                this->curr_node = other;
//            }

            bool operator==(const iterator &other) const
            {
                if(this->location == (*this->tree).get_order(this->type).size()
                   && other.location == (*other.tree).get_order(other.type).size())
                {
                    return true;
                }
                if(this->location == (*this->tree).get_order(this->type).size()
                   || other.location == (*other.tree).get_order(other.type).size())
                {
                    return false;
                }
                return this->get_curr_node() ==
                            other.get_curr_node();
            }

            bool operator!=(const iterator &other) const
            {
                return !(*this == other);
            }

            T &operator*() const
            {
                return this->get_curr_node()->value;
            }

            T *operator->() const
            {
                return &(this->get_curr_node()->value);
            }

        };

        BinaryTree<T>::iterator begin_preorder()
        {
            pre.clear();
            recursive_preorder(root);
            return iterator(this, OrderType::preorder);
        }
        BinaryTree<T>::iterator end_preorder()
        {
            begin_preorder();
            return (iterator(this,  OrderType::preorder, pre.size()));
        }

        BinaryTree<T>::iterator begin_inorder()
        {
            in.clear();
            recursive_inorder(root);
            return iterator(this,  OrderType::inorder);
        }

        BinaryTree<T>::iterator end_inorder()
        {
            begin_inorder();
            return (iterator(this, OrderType::inorder, in.size()));
        }

        BinaryTree<T>::iterator begin_postorder()
        {
            post.clear();
            recursive_postorder(root);
            return iterator(this,  OrderType::postorder);
        }

        BinaryTree<T>::iterator end_postorder()
        {
            begin_postorder();
            return (iterator(this, OrderType::postorder, post.size()));
        }

        BinaryTree<T>::iterator begin()
        {
            return begin_inorder();
        }

        BinaryTree<T>::iterator end()
        {
            return end_inorder();
        }

        BinaryTree<T> &add_left(T father, T left)
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("can't add a left child to a non existing tree");
            }
            iterator find_father = this->begin();
            for (; find_father != this->end(); ++find_father)
            {
                if (*(find_father) == father)
                {
                    Node *curr_node = find_father.get_curr_node();
                    if (curr_node->left_child == nullptr)
                    {
                        curr_node->left_child = new Node(left);
                    }
                    else
                    {
                        curr_node->left_child->value = left;
                    }
                    return *this;
                }
            }
            throw std::invalid_argument("father not found");
        }

        BinaryTree<T> &add_right(T root, T right)
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("can't add a right child to a non existing tree");
            }
            iterator find_root = this->begin();
            for (; find_root != this->end(); ++find_root)
            {
                if (*(find_root) == root)
                {
                    Node *curr_node = find_root.get_curr_node();
                    if (curr_node->right_child == nullptr)
                    {
                        curr_node->right_child = new Node(right);
                    }
                    else
                    {
                        curr_node->right_child->value = right;
                    }
                    return *this;
                }
            }
            throw std::invalid_argument("father not found");
        }

        // getters and setters
        Node *get_root() { return root; }

        Node *get_right(Node *father) { return father->right_child; }
        Node *get_left(Node *father) { return father->left_child; }
    };
};
