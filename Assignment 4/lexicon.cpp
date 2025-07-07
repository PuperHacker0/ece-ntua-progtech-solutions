#include <iostream>
using namespace std;

struct Node {
    string word = "";
    Node *left_child = nullptr, *right_child = nullptr;
    int count = 1;

    Node(const string &w, int cnt) { word = w, count = cnt; }
};

class lexicon {
public:
    lexicon() {}
    ~lexicon() {}
    
    void insert(const string &s, int multiple_count = 1) {
        if (root == nullptr) { //Special case, root itself has to change
            root = new Node(s, multiple_count);
            return;
        }
        
        Node *current = root; //Don't assign to this tmp variable unless it's by reference. Only edit objects through it

        while (1) {
            if (current->word == s) { //Our word already exists 
                current->count += multiple_count; //Increase word appearance count (by one if default, by k times if specified)
                return;
            }
            else if (current->word < s) { //Lexicographically smaller
                if (current->right_child == nullptr) {
                    current->right_child = new Node(s, multiple_count);
                    return;
                }
                else current = current->right_child;
            }
            else {
                if (current->left_child == nullptr) {
                    current->left_child = new Node(s, multiple_count);
                    return;
                }
                else current = current->left_child;
            }
        }
    }

    int lookup(const string &s) const {
        Node *current = root;
  
        while (current != nullptr) {
            if (current->word == s) //Found the word
                return current->count;
            else if (current->word < s)
                current = current->right_child;
            else
                current = current->left_child;
        }

        return 0; //If we're here, the place where our word should be is nullptr. So the word does not exist.
    }
    
    int depth(const string &s) const {
        Node *current = root;
        int d = 0;
  
        while (current != nullptr) {
            if (current->word == s) //Found the word
                return d;
            else if (current->word < s)
                current = current->right_child;
            else
                current = current->left_child;
            
                ++d; //Increase the word depth
        }

        return -1; //If we're here, the place where our word should be is nullptr. So the word does not exist.
    }
    
    void replace(const string &s1, const string &s2) {
        //Try to locate s1
        int s1_cnt = lookup(s1);

        if (s1_cnt == 0) return; //s1 is not present, so we dont have to add any occurances of s2

        root = deleteNode(root, s1); //Delete s1 and restore BST structure in the tree

        insert(s2, s1_cnt); //Insert k times the word s2
    }

    friend ostream& operator<<(ostream &out, const lexicon &l) {
        l.recursivePrint(out, l, l.root);
        return out;
    }

private:
    Node* root = nullptr;

    void recursivePrint(ostream &out, const lexicon &l, Node *current) const {
        //Inorder traversal ensures alphabetical ordering of words
        if (current == nullptr) return;

        recursivePrint(out, l, current->left_child);
        out << current->word << " " << current->count << endl;
        recursivePrint(out, l, current->right_child);
    }

    Node* deleteNode(Node *current, const string &s1) { //Deletes a node from the tree and fixes tree structure to maintain BST
        //***Reworked to be recursive because of the special case when the root has to be replaced

        if (current == nullptr) return current; //Node does not exist

        if (current->word > s1)
            current->left_child = deleteNode(current->left_child, s1);
        else if (current->word < s1)
            current->right_child = deleteNode(current->right_child, s1);
        else if (current->word == s1) { //Found the word
            //Subtree root is a leaf
            if (current->left_child == nullptr && current->right_child == nullptr) {
                delete current;
                return nullptr;
            }
            //Subtree root has only right child
            else if (current->right_child != nullptr && current->left_child == nullptr) {
                Node* temp = current->right_child; //Keep the pointer to the child
                delete current; //Delete the root
                return temp; //Return the child as the new root
            }
            //Subtree root has only left child
            else if (current->left_child != nullptr && current->right_child == nullptr) {
                Node* temp = current->left_child; //Keep the pointer to the child
                delete current; //Delete the root
                return temp; //Return the child as the new root
            }
            else if (current->left_child != nullptr && current->right_child != nullptr) {
                //Subtree root has both children (difficult case)
                //First, locate the word closest to this one lexicographically in the tree
                //One option is the rightmost word of the left subtree, which is the word immediately before the root
                Node *successor = current->left_child;
                while (successor->right_child != nullptr) //There is a node more to the bottom-right
                    successor = successor->right_child;
                
                //Copy successor data to current
                current->word = successor->word; current->count = successor->count;
                
                //Now delete the successor
                current->left_child = deleteNode(current->left_child, successor->word);
            }
        }

        return current;
    }
};

/*int main() {
    lexicon l;
    l.insert("the");
    l.insert("boy");
    l.insert("and");
    l.insert("the");
    l.insert("wolf");
    
    cout << "The word 'the' is found " << l.lookup("the") << " time(s)" << endl;
    cout << "The word 'and' is found at depth " << l.depth("and") << endl;
    cout << l;
    
    l.replace("boy", "wolf");
    cout << "After replacement:\n";
    cout << l;
    cout << "Now the word 'and' is found at depth " << l.depth("and") << endl;
 
    l.replace("and", "dummy");
    l.replace("boy", "dummy");
    l.replace("the", "dummy");
    l.replace("wolf", "dummy");
    cout << l;

    return 0;
}*/