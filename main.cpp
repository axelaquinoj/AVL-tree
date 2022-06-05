#include <iostream>
#include "string"
#include "sstream"
#include "vector"

using namespace std;

/* Note:
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file on canvas. If you are using multiple header files,
	   you will have to include them directly in main.cpp so we can test your code as a single file.
*/
struct Node {
    int id;
    string name;
    int height;
    Node *left;
    Node *right;
    int bf;

    Node() {}

    Node(string &name, int id) {
        this->name = name;
        this->id = id;
        left = nullptr;
        right = nullptr;
    }

    int getHeight(Node *root) {
        if (root == nullptr) {
            return 0;
        }
        if (root->left == nullptr && root->right == nullptr) {
            height = 1;
        } else {
            height = 1 + max(getHeight(root->left), getHeight(root->right));
        }
        return height;
    }

    int getBalanceFactor() {
        bf = getHeight(left) - getHeight(right);
        return bf;
    }


};

class AVLTree {
    Node *root = nullptr;
    bool duplicate;
    bool found;
    vector<int> idNums;
    vector<string> names;

    void removeInorder(Node *root) {
        if (root == nullptr)
            return;
        else {
            removeInorder(root->left);
            idNums.push_back(root->id);
            removeInorder(root->right);
        }
    }

    Node *inorderSuccessor(Node *root) {
        Node *successor = root->right;
        while (true) {
            if (successor->left == nullptr) {
                return successor;
            }
            successor = successor->left;
        }
    }

    Node *getParent(Node *root) {
        Node *parent = this->root;
        while (true) {
            if (parent->left == root || parent->right == root) {
                return parent;
            }
            if (root->id < parent->id) {
                parent = parent->left;
            } else {
                parent = parent->right;
            }
        }
    }

    Node *rotateLeft(Node *node) {
        Node *grandchild = node->right->left;
        Node *newParent = node->right;
        newParent->left = node;
        node->right = grandchild;
        return newParent;
    }

    Node *rotateRight(Node *node) {
        Node *grandchild = node->left->right;
        Node *newParent = node->left;
        newParent->right = node;
        node->left = grandchild;
        return newParent;
    }

    Node *insert(Node *root, string &name, int id) {
        if (root == nullptr) {
            return new Node(name, id);
        } else if (root->id == id) {
            duplicate = true;
        } else if (id < root->id) {
            root->left = insert(root->left, name, id);
        } else {
            root->right = insert(root->right, name, id);
        }

        if (root->getBalanceFactor() == -2) {
            if (root->right->getBalanceFactor() == 1) {
                root->right = rotateRight(root->right);
                root = rotateLeft(root);
            } else {
                root = rotateLeft(root);
            }
        } else if (root->getBalanceFactor() == 2) {
            if (root->left->getBalanceFactor() == -1) {
                root->left = rotateLeft(root->left);
                root = rotateRight(root);
            } else {
                root = rotateRight(root);
            }
        }
        return root;
    }

    Node *searchID(Node *root, int id) {
        if (root == nullptr) {
            cout << "unsuccessful" << endl;
            return nullptr;
        } else if (root->id == id) {
            cout << root->name << endl;
        } else if (id < root->id) {
            root->left = searchID(root->left, id);
        } else {
            root->right = searchID(root->right, id);
        }
        return root;
    }

    void searchName(Node *root, string &name) {
        if (root == nullptr) {
            return;
        } else if (root->name == name) {
            cout << root->id << endl;
            found = true;
        }
        searchName(root->left, name);
        searchName(root->right, name);
    }

    void inOrder(Node *root) {
        if (root == nullptr)
            return;
        else {
            inOrder(root->left);
            names.push_back(root->name);
            inOrder(root->right);
        }
    }

    void preOrder(Node *root) {
        if (root == nullptr)
            return;
        else {
            names.push_back(root->name);
            preOrder(root->left);
            preOrder(root->right);
        }
    }

    void postOrder(Node *root) {
        if (root == nullptr)
            return;
        else {
            postOrder(root->left);
            postOrder(root->right);
            names.push_back(root->name);
        }
    }

    Node *remove(Node *root, int id) {
        if (root == nullptr) {
            return nullptr;
        }
        if (id < root->id) {
            root->left = remove(root->left, id);
        } else if (id > root->id) {
            root->right = remove(root->right, id);
        } else {
            found = true;
            Node *parent = nullptr;
            if (root->left == nullptr && root->right == nullptr) {
                delete root;
                return nullptr;
            } else if ((root->left == nullptr && root->right != nullptr) ||
                       (root->left != nullptr && root->right == nullptr)) {
                if (root->left == nullptr) {
                    parent = root->right;
                    delete root;
                    return parent;
                } else {
                    parent = root->left;
                    delete root;
                    return parent;
                }
            } else {
                Node *successor = inorderSuccessor(root);
                parent = getParent(successor);
                root->id = successor->id;
                root->name = successor->name;
                if (parent->id < successor->right->id) {
                    parent->right = successor->right;
                } else {
                    parent->left = successor->right;
                }
                delete successor;
            }

        }
        return root;
    }

public:
    AVLTree() {};

    bool insert(string &name, int id) {
        duplicate = false;
        root = insert(root, name, id);
        return !duplicate;

    }

    void searchID(int id) {
        root = searchID(root, id);
    }

    void searchName(string &name) {
        found = false;
        searchName(root, name);
        if (!found) {
            cout << "unsuccessful" << endl;
        }
    }

    int getLevelCount() {
        return root->getHeight(root);
    }

    void printInorder() {
        names.clear();
        inOrder(root);
        for (int i = 0; i < names.size(); ++i) {
            cout << names[i];
            if (i != names.size()-1){
                cout << ", ";
            }
        }
    }

    void printPreOrder() {
        names.clear();
        preOrder(root);
        for (int i = 0; i < names.size(); ++i) {
            cout << names[i];
            if (i != names.size()-1){
                cout << ", ";
            }
        }
    }

    void printPostOrder() {
        names.clear();
        postOrder(root);
        for (int i = 0; i < names.size(); ++i) {
            cout << names[i];
            if (i != names.size()-1){
                cout << ", ";
            }
        }
    }

    bool remove(int id) {
        found = false;
        root = remove(root, id);
        return found;
    }

    bool removeInorder(int num) {
        idNums.clear();
        removeInorder(root);
        for (int i = 0; i < idNums.size(); ++i) {
            if (i == num) {
                return remove(idNums[i]);
            }
        }
        return false;
    }

};

bool verifyData(string &name, string &id);

bool isNumber(string &data);

int main() {
    AVLTree tree;
    int numCommands;
    cin >> numCommands;
    for (int i = 0; i <= numCommands; ++i) {
        string line;
        getline(cin, line);
        istringstream stream(line);
        string name;
        string id;
        string data;
        string command;
        stream >> command;
        string space;

        if (command == "insert") {
            getline(stream, space, '"');
            getline(stream, name, '"');
            getline(stream, space, ' ');
            getline(stream, id);
            if (verifyData(name, id)) {
                if (tree.insert(name, stoi(id))) {
                    cout << "successful" << endl;
                } else {
                    cout << "unsuccessful" << endl;
                }
            } else {
                cout << "unsuccessful" << endl;
            }

        } else if (command == "remove") {
            getline(stream, space, ' ');
            getline(stream, id);
            if (isNumber(id) && tree.remove(stoi(id))) {
                cout << "successful" << endl;
            } else {
                cout << "unsuccessful" << endl;
            }
        } else if (command == "search") {
            getline(stream, space, ' ');
            getline(stream, data);
            if (isNumber(data)) {
                tree.searchID(stoi(data));
            } else {
                name = data.substr(1, data.length() - 2);
                tree.searchName(name);
            }

        } else if (command == "printInorder") {
            tree.printInorder();
            cout << endl;
        } else if (command == "printPreorder") {
            tree.printPreOrder();
            cout << endl;
        } else if (command == "printPostorder") {
            tree.printPostOrder();
            cout << endl;
        } else if (command == "printLevelCount") {
            cout << tree.getLevelCount() << endl;
        } else if (command == "removeInorder") {
            getline(stream, space, ' ');
            getline(stream, data);
            if (isNumber(data) && tree.removeInorder(stoi(data))) {
                cout << "successful" << endl;
            } else {
                cout << "unsuccessful" << endl;
            }
        }
    }

    return 0;
}

bool verifyData(string &name, string &id) {
    if (id.length() != 8) {
        return false;
    }
    for (char c: name) {
        if (!isalpha(c) && !isspace(c)) {
            return false;
        }
    }
    for (char c: id) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isNumber(string &data) {
    for (char c: data) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}
