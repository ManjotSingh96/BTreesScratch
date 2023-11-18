//import all the necessary libraries
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

// def a struct representing a node in a tree
struct Node {
    // store keys and child nodes for each node
    std::vector<int> keys; // keys with this node
    std::vector<Node*> children; // child of this node
    bool leaf; // bool flag if the node is a leaf
    Node() {
        // intiazlize node with the leaf set to true
        leaf = true;
    }
};


// function to check if balanced
bool checkBalance(Node* root, int depth, int& leaf_depth) {
    // check if root = null
    if (!root)
        return true;
    
    // check if root is leaf node
    if (root->leaf) {
        // if leaf depth is not set
        if (leaf_depth == -1) {
            // set leaf depth to current depth
            leaf_depth = depth;
            // return true
            return true;
        } else {
            // check current depth is same as leaf depth
            return depth == leaf_depth;
        }
    }
    
    // initialize bool for balance
    bool is_balanced = true;
    
    // iterate through children of root
    for (Node* child : root->children) {
        // check balance for each child
        is_balanced = is_balanced && checkBalance(child, depth + 1, leaf_depth);
    }
    // return the balance status
    return is_balanced;
}

// search for keys in range
void search(Node* root, int lower, int upper, std::vector<int>& result) {
    // check if root is null
    if (!root)
        return;
    
    // initialize index variable
    int i = 0;
    
    // iterate until index is within range and less than lower limit
    while (i < root->keys.size() && root->keys[i] < lower)
        i++;
    
    // check if root is leaf
    if (root->leaf) {
        // iterate through keys in root within range
        while (i < root->keys.size() && root->keys[i] <= upper) {
            // push keys within range to vector
            result.push_back(root->keys[i]);
            i++;
        }
        // return leaf search is complete
        return;
    }
    
    // iterate through keys in root and call search for children
    while (i < root->keys.size()) {
        search(root->children[i], lower, upper, result);
        i++;
    }
    
    // call search for last child of root
    search(root->children[i], lower, upper, result);
}

// function to split child of parent at the index
void split_child(Node* parent, int index) {
    // get child at index from parent 
    Node* child = parent->children[index];

    // create new node
    Node* new_node = new Node();
    // set leaf property new node to match childs leaf 
    new_node->leaf = child->leaf;

    // calc midpoint of the child node keys to split key
    int mid = child->keys.size() / 2;

    // copy keys from child node to new node from midpoint to end
    for (int j = mid + 1; j < child->keys.size(); j++)
        new_node->keys.push_back(child->keys[j]);

    // check if child is not leaf node
    if (!child->leaf) {
        // copy children from child node to new node from midpoint to end
        for (int j = mid + 1; j <= child->keys.size(); j++)
            new_node->children.push_back(child->children[j]);
    }

    // resize child nodes key to only contain key up to midpoint
    child->keys.resize(mid);
    // resize child nodes children contain appropriate number of children
    child->children.resize(mid + 1);

    // insert key at midpoint of child node into parent node at specified index
    parent->keys.insert(parent->keys.begin() + index, child->keys[mid]);
    // insert new node as a child of parent node at index after specified index
    parent->children.insert(parent->children.begin() + index + 1, new_node);
}


// function to insert key into a not full node in B-tree
void insert_non_full(Node* node, int key) {
    // initialize index to last element of keys vector in node
    int i = node->keys.size() - 1;

    // check if node is leaf node
    if (node->leaf) {
        // add placeholder value to keys vector
        node->keys.push_back(0);

        // shift keys to make space for new key in position
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        // insert new key at position in keys vector
        node->keys[i + 1] = key;
    } else {
        // find child index to insert new key
        while (i >= 0 && key < node->keys[i])
            i--;

        // check if child at index i +1 is full
        if (node->children[i + 1]->keys.size() == 5) {
            // split child if full
            split_child(node, i + 1);

            // check if new key is > key at split position
            if (key > node->keys[i + 1])
                i++;
        }

        // insert new key in child node
        insert_non_full(node->children[i + 1], key);
    }
}

// function to print keys of B-tree in a tree format
void printTree(Node* root, int depth = 0) {
    // check if root is null 
    if (!root) return;

    // print tabs for current depth
    for (int i = 0; i < depth; ++i) std::cout << "\t";

    // print keys at current node
    for (int i = 0; i < root->keys.size(); ++i) {
        std::cout << root->keys[i] << " ";
    }
    std::cout << std::endl;

    // check if current node is not leaf node
    if (!root->leaf) {
        // call function for each child node at next depth level
        for (int i = 0; i < root->children.size(); ++i) {
            printTree(root->children[i], depth + 1);
        }
    }
}


// main function creates B-tree searches for keys in a certain range checks balance of tree
int main() {
    int N;

    // loop until a valid value of N
    while (true) {
        std::cout << "Enter the value of N (N >= 400): ";
        std::cin >> N;
        if (N >= 400) {
            break;
        } else {
            std::cout << "Please enter a value greater than or equal to 400." << std::endl;
        }
    }

    // initazlize random number generation with seed based on current time
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> distribution(0, 3 * N);
    std::vector<int> keys;

    // generate N random keys and store them in vector
    for (int i = 0; i < N; i++) {
        keys.push_back(distribution(rng));
    }

    // create new B tree node as root
    Node* root = new Node();

    // insert generated keys into B tree
    for (int i = 0; i < keys.size(); i++) {
        insert_non_full(root, keys[i]);
    }

    // search for keys in specified range store them in vector
    std::vector<int> keysInRange;
    search(root, N, 2 * N, keysInRange);
    std::cout << "Output:\n\n(a) the tree:\n";

    // print tree in a tree format
    printTree(root, 0);
    std::cout << "\n\n(b) array of keys found:\n" ;

    // check if vector of keys in specified range is empty
    if (keysInRange.empty()) {
        std::cout << "(empty array or null for no keys found)";
    } else {
        // print keys in vector
        for (int i = 0; i < keysInRange.size(); i++) {
            std::cout << keysInRange[i] << " ";
        }
    }
    std::cout << "\n\n";

    // initialize leaf depth check if B tree is balanced
    int leaf_depth = -1;
    bool isBalanced = checkBalance(root, 0, leaf_depth);
    std::cout << "Is the tree balanced? " << (isBalanced ? "Yes" : "No") << "\n";

    return 0;
}

