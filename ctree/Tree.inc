#include "Tree.h"
#include <iostream>
#include <string>
#include <sstream>

// constructor with NULL as default values
template<typename T>
Tree<T> :: Tree(T data) : data(data), kids(nullptr), sibs(nullptr), prev(nullptr) { }

// copy constructor
template<typename T>
Tree<T> :: Tree(T data, Tree<T> *t_kids, Tree<T> *t_sibs, Tree<T> *t_prev): data(data), kids(t_kids), sibs(t_sibs), prev(t_prev) { }

// destructor
template<typename T>
Tree<T> :: ~Tree() {
    delete sibs;
    delete kids;
}

// implement first addSibling function
template<typename T>
bool Tree<T> :: addSibling(T data) {
    // prevent adding a sibling to the root node of the entire tree
    if (this->prev == nullptr) {
        return false;
    }

    // store data in a new node
    Tree<T>* node_t = new Tree<T>(data); // set kids, sibs, prev to nullptr
    this->addSibling(node_t);

    // check if new node was made and data was stored successfully
    if ((node_t == nullptr) || (node_t->data != data)) {
        return false;
    }

    return true;
}

// overloaded, second addSibling function
template<typename T>
bool Tree<T> :: addSibling(Tree *root) {
    Tree<T> *current = this;
    // check if data is already a sibling
    if (root->data == current->data) {
        return false;
    }

    if (root->data < current->data) {
        while(root->data < current->data) {
            // exit loop if current is a first child
            if (current == (current->prev)->kids) {
                break;
            }
            current = current->prev;
            // check if root already exists as a sibling left of current
            if (current->data == root->data) {
                return false;
            }
        }
        // add root as first child
        if(current == (current->prev)->kids) {
            (current->prev)->kids = root;
            root->prev = current->prev;
            root->sibs = current;
            current->prev = root;
        }
        // add root between two pre-existing siblings
        else if (current->data < root->data && root->data < (current->sibs)->data) {
            root->sibs = current->sibs;
            (current->sibs)->prev = root;
            current->sibs = root;
            root->prev = current;
        }

        return true;
    }
    // check if root already exists right of current
    else if (root->data > current->data) {
        while (root->data > current->data) {
            // check if current is the last sibling
            if (current->sibs == nullptr) {
                break;
            }
            current = current->sibs;
            // check if root already exists as a sibling right of current
            if (current->data == root->data) {
                return false;
            }
        }
        // make root the last sibling
        if (current->sibs == nullptr && root->data > current->data) {
            // add root to the right of current
            // root->sibs is nullptr by default
            root->prev = current;
            current->sibs = root;
        }
        else {
            // add root to the left of current
            root->prev = current->prev;
            root->sibs = current;
            (current->prev)->sibs = root;
            current->prev = root;
        }

        return true;
    }
    return true;
}

template<typename T>
bool Tree<T> :: addChild(T data) {
    // add child if parent has no children
    if (this->kids == nullptr) {
        // make a new node for data
        Tree<T>* node_t = new Tree<T>(data); // set kids, sibs, prev to nullptr
        this->addChild(node_t);

        // check if new node was made and data was stored successfully
        if ((node_t == nullptr) || (node_t->data != data)) {
            return false;
        }

        return true;
    }
    
    Tree *current = this;

    current = current->kids; // move to first kid (which is the first sib)

    // iterate to check that node with data does not exist amongst siblings
    while(current->sibs != nullptr) {
        if (current->data != data) {
            current = current->sibs; // move to next sibling
        }
        else {
            return false;
        }
    }

    // check that last sibling does not contain data as data
    if (current->data == data) {
        return false;
    }

    Tree<T>* node_t = new Tree<T>(data);
    this->addChild(node_t);

    return true;
}
template<typename T>
bool Tree<T> :: addChild(Tree<T> *root) {
    // check if root has prev or siblings
    if (root->prev != nullptr || root->sibs != nullptr) {
        return false;
    }

    // if parent has no kids, add kids immediately
    if(this->kids == nullptr) {
        this->kids = root;
        root->prev = this;
    }

    // use addSibling to add new child
    (this->kids)->addSibling(root);

    return true;
}

template<typename T>
std::string Tree<T> :: toString() {
    std::stringstream ss;
    ss << this->data << "\n";

    if (this->kids != nullptr) {
        ss << (this->kids)->toString();
    }
    if (this->sibs != nullptr) {
        ss << (this->sibs)->toString();
    }

    // convert stringstream object into a string
    return ss.str();
}

// use addChild function to overload + operator
template<typename T>
Tree<T>& Tree<T> :: operator+(Tree<T>& rt) {
    this->addChild(&rt);
    return *this;
}

template<typename T>
bool Tree<T> :: treeCompare(const Tree<T> *a, const Tree<T> *b) const {
    // return false if any of the two is a nullptr
    if (a == nullptr && b == nullptr) {
        return false;
    }

    // catch when only one of the two children is a nullptr
    if (!(a->kids == nullptr) && b->kids == nullptr) {
        return false;
    }
    else if(a->kids == nullptr && !(b->kids == nullptr)) {
        return false;
    }

    // compare children's data when both are not nullptrs
    if (a->kids != nullptr && b->kids != nullptr){
        if ((a->kids)->data != (b->kids)->data) {
            return false;
        }
        else {
            return treeCompare(a->kids, b->kids); // use recursion to traverse
        }
    }

    // catch when only one of the two sibs is a nullptr
    if (!(a->sibs == nullptr) && b->sibs == nullptr) {
        return false;
    }
    else if (a->sibs == nullptr && !(b->sibs == nullptr)) {
        return false;
    }

    // compare sibs' data when both are not nullptrs
    if (a->sibs != nullptr && b->sibs != nullptr) { 
        if ((a->sibs)->data != (b->sibs)->data) {
            return false;
        }
        else {
            return treeCompare(a->sibs, b->sibs);
        }
    }

    return true;
}

// use helper function treeCompare to overload == operator
template<typename T>
bool Tree<T> :: operator==(const Tree<T> &root) {
    return this->treeCompare(this, &root);
}

// use toString function to overload << operator
template<typename T>
std::ostream& operator<<(std::ostream& os, Tree<T>& rt) {
    os << rt.toString();
    return os;
}