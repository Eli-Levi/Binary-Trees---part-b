#include <iostream>
#include <stdlib.h>
#include <stack>
#include <string>

namespace ariel
{
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
            if (other == nullptr)
            {
                return;
            }
            store_where = new Node(other->value);
            tree_copy(store_where->right_child, other->right_child);
            tree_copy(store_where->left_child, other->left_child);
        }

        Node *root;

    public:
        BinaryTree() = default;

        BinaryTree(BinaryTree &other) //ctor constructor
        {
//            this->root = new Node(*(other.root));
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
                free_memory(this->root);
                tree_copy(this->root,copy_from.root);
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
        friend std::ostream &operator<<(std::ostream &os, BinaryTree<T> &print)
        {
            return os;
        }


        class iterator
        {
            Node *curr_node;
            std::stack<Node *> travel;

            void recursive_inorder(Node *next)
            {
                if (next == nullptr)
                {
                    return;
                }
                recursive_inorder(next->right_child);
                travel.push(next);
                recursive_inorder(next->left_child);
            }

            void recursive_preorder(Node *next)
            {
                if (next == nullptr)
                {
                    return;
                }
                recursive_inorder(next->right_child);
                recursive_inorder(next->left_child);
                travel.push(next);
            }

            void recursive_postorder(Node *next)
            {
                if (next == nullptr)
                {
                    return;
                }
                travel.push(next);
                recursive_inorder(next->right_child);
                recursive_inorder(next->left_child);
            }

        public:
            explicit iterator(const std::string &iter_type = "inorder", Node *ptr = nullptr)
            {
                if (ptr != nullptr)
                {
                    if (iter_type == "inorder")
                    {
                        recursive_inorder(ptr);
                    }
                    else if (iter_type == "postorder")
                    {
                        recursive_postorder(ptr);
                    }
                    else if (iter_type == "preorder")
                    {
                        recursive_preorder(ptr);
                    }
                    this->curr_node = travel.top();
                }
                else
                {
                    this->curr_node = ptr;
                }
            }

            iterator &operator++() // ++i
            {
                travel.pop();
                if (travel.empty())
                {
                    curr_node = nullptr;
                }
                else
                {
                    curr_node = travel.top();
                }
                return *this;
            }

            iterator operator++(int) // i++
            {
                iterator tmp = *this;
                travel.pop();
                if (travel.empty())
                {
                    curr_node = nullptr;
                }
                else
                {
                    curr_node = travel.top();
                }
                return tmp;
            }

            iterator &operator=(T *other)
            {
                this->curr_node = other;
            }

            bool operator==(const iterator &other) const { return curr_node == other.curr_node; }

            bool operator!=(const iterator &other) const { return curr_node != other.curr_node; }

            T &operator*() const
            {
                return curr_node->value;
            }

            T *operator->() const
            {
                return &(curr_node->value);
            }

            Node *get_curr_node() const
            {
                return this->curr_node;
            }

            std::stack<T *> &get_tree() { return travel; }
            friend std::ostream &operator<<(std::ostream &os, iterator &print) { return os; }
        };

        BinaryTree<T>::iterator begin_preorder()
        {
            return iterator("preorder", this->root);
        }
        BinaryTree<T>::iterator end_preorder()
        {
            return (iterator("preorder"));
        }

        BinaryTree<T>::iterator begin_inorder()
        {
            return iterator("inorder", this->root);
        }

        BinaryTree<T>::iterator end_inorder()
        {
            return iterator();
        }

        BinaryTree<T>::iterator begin_postorder()
        {
            return iterator("postorder", this->root);
        }

        BinaryTree<T>::iterator end_postorder()
        {
            return iterator("postorder");
        }

        BinaryTree<T>::iterator begin()
        {
            return iterator("inorder", this->root);
        }

        BinaryTree<T>::iterator end()
        {
            return iterator();
        }

        BinaryTree<T> &add_left(T root, T left)
        {
            if (this->root == nullptr)
            {
                throw std::invalid_argument("can't add a left child to a non existing tree");
            }

            iterator find_root = this->begin();
            for (; find_root != this->end(); ++find_root)
            {
                if (*(find_root) == root)
                {
                    Node *curr_node = find_root.get_curr_node();
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
            throw std::invalid_argument("root was not found");
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
            throw std::invalid_argument("root was not found");
        }

        // getters and setters
        Node *get_root() { return root; }

        Node *get_right(Node *father) { return father->right_child; }
        Node *get_left(Node *father) { return father->left_child; }
    };
};
