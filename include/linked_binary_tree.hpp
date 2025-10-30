//linked_binary_tree.hpp
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

template <typename E>
class LinkedBinaryTree{
    protected:
        class Node {
            // practice writing code yourself and match with slides
            public:
                E element; // element value
                Node* parent; // parent node
                Node* left; // left child
                Node* right; // right child

                Node(const E& e, Node* p=nullptr, Node* l=nullptr, Node* r=nullptr)
                    : element{e}, parent{p}, left{l}, right{r} {}
        };// end of Node class

    protected:
        Node* rt{nullptr}; // root
        int sz{0}; // size

    public:
        class Position {
            private:
                // practice writing code yourself and match with slides
                Node* node; // pointer to the node being wrapped
                LinkedBinaryTree* tree; // pointer to the tree containing the nod
            public:
                // Wraps a Node* pointer 
                //(default is nullptr)
                Position(Node* nd=nullptr): node{nd} {};

                // operator (based on node pointer)
                bool operator==(Position other) const{
                    // practice writing code yourself and match with slides
                    return node == other.node
                }
                bool operator!=(Position other) const{
                    // practice writing code yourself and match with slides
                    return node != other.node;
                }
                bool is_null() const{
                    // practice writing code yourself and match with slides
                    return node == nullptr;
                }
                bool is_root() const{
                    // practice writing code yourself and match with slides
                    return node != nullptr && node->parent == nullptr;
                }
                bool is_external() const{
                    // practice writing code yourself and match with slides
                    return node != nullptr && node->left == nullptr && node->right == nullptr;
                }
                E& element(){ // get reference to element
                    // practice writing code yourself and match with slides
                    return node->element;
                }
                const E& element() const{
                    // practice writing code yourself and match with slides
                    return node->element;
                }

                Position parent() const{
                    // practice writing code yourself and match with slides
                    return Position(node->parent);
                }
                Position left() const{
                    // practice writing code yourself and match with slides
                    return Position(node->left);
                }
                Position right() const{
                    // practice writing code yourself and match with slides
                    return Position(node->right);
                }
                
                std::vector<Position> children() const{
                    // practice writing code yourself and match with slides
                    std::vector<Position> result;
                    if(node->left != nullptr)
                        result.push_back(Position(node->left));
                    if(node->right != nullptr)
                        result.push_back(Position(node->right));
                    return result;
                }
                int num_children() const{
                    // practice writing code yourself and match with slides
                    int count = 0;
                    if(node->left != nullptr)
                        count++;
                    if(node->right != nullptr)
                        count++;
                    return count;
                }
        };  // end of Position class
        LinkedBinaryTree() = default; //tree with zero nodes
        int size() const{
            // practice writing code yourself and match with slides
            return sz;
        }
        bool empty() const{
            // practice writing code yourself and match with slides
            return sz == 0;
        }
        Position root() const{
            // practice writing code yourself and match with slides
            return Position(rt);
        }
        std::vector<Position> positions() const{
            // practice writing code yourself and match with slides
            std::vector<Position> pos;
            if (rt != nullptr)
                preorder_positions(Position(rt), pos);
            return pos;
    
        }
        void add_root(const E& e=E()){
            // practice writing code yourself and match with slides
            rt = new Node(e);
            sz = 1;
        }
        void add_left(Position p, const E& e){
            // practice writing code yourself and match with slides
            Node* newNode = new Node(e, p.node);
            p.node->left = newNode;
            sz++;
        }
        void add_right(Position p, const E& e){
            // practice writing code yourself and match with slides
            Node* newNode = new Node(e, p.node);
            p.node->right = newNode;
            sz++;
        }

        void erase(Position p){
            // practice writing code yourself and match with slides
            Node* nd = p.node;
            if (nd->left != nullptr || nd->right != nullptr)
                throw std::runtime_error("Cannot erase node with children");
            
            Node* par = nd->par;
            if (par != nullptr) {
                if (par->left == nd)
                    par->left = nullptr;
                else
                    par->right = nullptr;
            } else {
                rt = nullptr;
            }
            delete nd;
            sz--;
        }

        void attach(Position p, LinkedBinaryTree& left, LinkedBinaryTree& right){
            // practice writing code yourself and match with slides
             Node* nd = p.node;
            if (nd->left != nullptr || nd->right != nullptr)
                throw std::runtime_error("Position already has children");
            
            nd->left = left.rt;
            if (left.rt != nullptr) {
                left.rt->par = nd;
                sz += left.sz;
                left.rt = nullptr;
                left.sz = 0;
            }
            
            nd->right = right.rt;
            if (right.rt != nullptr) {
                right.rt->par = nd;
                sz += right.sz;
                right.rt = nullptr;
                right.sz = 0;
            }
        }
    // ------------- Rule-of-five support ----------------
    private:
        void tear_down(Node* nd){
            // practice writing code yourself and match with slides
            if (nd != nullptr) {
                tear_down(nd->left);
                tear_down(nd->right);
                delete nd;
            }
        }
        static Node* clone(Node* model){
            // practice writing code yourself and match with slides
            if (model == nullptr)
                return nullptr;
            Node* copy = new Node(model->elem);
            copy->left = clone(model->left);
            copy->right = clone(model->right);
            return copy;
        }

    public:
        ~LinkedBinaryTree(){
            // practice writing code yourself and match with slides
            tear_down(rt);
        }
        
        // copy constructor and copy assignment
        LinkedBinaryTree(const LinkedBinaryTree& other){
            // practice writing code yourself and match with slides
            rt = clone(other.rt);
            sz = other.sz;
        }

        LinkedBinaryTree& operator=(const LinkedBinaryTree& other){
            // practice writing code yourself and match with slides
            if (this != &other) {
                tear_down(rt);
                rt = clone(other.rt);
                sz = other.sz;
            }
            return *this;
        }

        // move constructor and move assignment
        LinkedBinaryTree(LinkedBinaryTree&& other){
            // practice writing code yourself and match with slides
            rt = other.rt;
            sz = other.sz;
            other.rt = nullptr;
            other.sz = 0;
        }

        LinkedBinaryTree& operator=(LinkedBinaryTree&& other){
            // practice writing code yourself and match with slides
            if (this != &other) {
                tear_down(rt);
                rt = other.rt;
                sz = other.sz;
                other.rt = nullptr;
                other.sz = 0;
            }
            return *this;
        }

    // ******************* Assignment ******************* 
    private:
        // Helper functions in case you want to write
        int count_left_leaves_helper(Node* nd, bool is_left_child) const {
            if (nd == nullptr)
                return 0;
            if (nd->left == nullptr && nd->right == nullptr && is_left_child)
                return 1;
            return count_left_leaves_helper(nd->left, true) + count_left_leaves_helper(nd->right, false);
        }

    public:
        int count_left_leaves() const{
            // ToDo (use recursion)
            //count nodes that are leaves in a binary tree and that are the left child of their respective parent. The root is not counted.
            if(rt == nullptr)
                return 0;
            return count_left_leaves_helper(rt->left, true) + count_left_leaves_helper(rt->right, false);
        }

        int count_left_leaves_bfs() const {
            // ToDo (use breadth first search)
            //count nodes that are leaves in a binary tree and that are the left child of their respective parent. The root is not counted.
            if (rt == nullptr)
                return 0;
            
            int count = 0;
            std::queue<Node*> q;
            q.push(rt);
            
            while (!q.empty()) {
                Node* current = q.front();
                q.pop();
                
                if (current->left != nullptr) {
                    if (current->left->left == nullptr && current->left->right == nullptr)
                        count++;
                    q.push(current->left);
                }
            
                if (current->right != nullptr) {
                    q.push(current->right);
                }
            }
            
            return count;
        }
    };