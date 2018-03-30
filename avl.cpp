#include <stdio.h>
#include <iostream>
#include "avl.hpp"
using namespace std;

#define IS_ROOT 0
#define IS_LEFT 1
#define IS_RIGHT 2

void print(AvlNode * root);
int tree_height(AvlNode * tmp);
int height_diff(AvlNode * tmp);
AvlNode * find_min_val(AvlNode * root);
AvlNode * ll_rotation(AvlNode * root);
AvlNode * lr_rotation(AvlNode * root);
AvlNode * rr_rotation(AvlNode * root);
AvlNode * rl_rotation(AvlNode * root);
AvlNode * balance(AvlNode * root);
AvlNode * update_heights(AvlNode * tmp);
// Insert a new node into the AVL tree
// int iter = keeps track of how many times
// we've had to move down (iterations), 
// Use for updating heights as we insert
void insert( const int & info, AvlNode * & root, int iter)
{
    if (root == nullptr)
    {
        if(iter == -1)
            iter = 0;
        cout << "[Inserting " << info << "]\n";
        root = new AvlNode (info, nullptr, nullptr, iter);
        return;
    }

    
    if (info < root->element)
        insert( info, root->left, tree_height(root)-1);
    
    else if (info > root->element) 
        insert( info, root->right, tree_height(root)-1);

    root = balance(root);

    
}

// Delete element in the AVL tree
AvlNode * remove( const int & info, AvlNode * & root )
{
    // Look for the element to remove
    if(root == NULL)
        return NULL;
    else if(info < root->element)
    {
        remove(info, root->left);
        root = balance(root);
    }
    else if(info > root->element)
    {
        remove(info, root->right);
        root = balance(root);
    }        

    // Several cases for deletion; no children, one children, and two children
    else
    {
        // No children
        if(root->left == NULL && root->right == NULL)
        {
            cout << "[Deleting " << root->element << ", has no children or is duplicate (moved to be a parent)]\n" << endl;
            delete root;
            root = NULL;
        }
        // One child, on left
        else if(root->left != NULL && root->right == NULL)
        {
            cout << "[Deleting " << root->element << "]" << endl;
            AvlNode * tmp = root;
            root = root->left;
            delete tmp;
        }
        // One child, on right
        else if(root->left == NULL && root->right != NULL)
        {
            cout << "[Deleting " << root->element << "]" << endl;
            AvlNode * tmp = root;
            root = root->right;
            delete tmp;
        }
        // Two children
        else if(root->left != NULL && root->right != NULL)
        {
            cout << "[Deleting " << root->element << "]" << endl;
            // Find the min val on the right subtree to make our new parent
            // of the two children whose parent we just deleted
            AvlNode * tmp = find_min_val(root->right);
            root->element = tmp->element;
            // Delete the duplicate min val that we just moved
            root->right = remove(tmp->element, root->right);
            // Rebalance tree, if necessary.
        }
    }
    
    return root;
}

// Returns the node containing the biggest value of the right subtree; used in deletion
// in case of a node deletion containing two children. This will guarantee the BST property
// is maintained (< node, node, > node)
AvlNode * find_min_val(AvlNode * root)
{
    if(root == NULL)
        return NULL;
    else
    {
        while(root->left != NULL)
            root = root->left;

        return root;
    }
}

// Returns the max height of the AVL tree
int tree_height(AvlNode * tmp)
{
    if(tmp == NULL || (tmp->left == NULL && tmp->right == NULL))
        return 0;
    else if(tmp->left == NULL && tmp->right != NULL)
        return tmp->right->height+1;
    else if(tmp->left != NULL && tmp->right == NULL)
        return tmp->left->height+1;
    else
        return 1 + max(tmp->left->height, tmp->right->height);
}

// Returns the height difference of the left and right subtree of root
int height_diff(AvlNode * tmp)
{
    if(tmp == NULL)
        return -1;

    int l_height = 1 + tree_height(tmp->left);
    int r_height = 1 + tree_height(tmp->right);
    int h_diff = l_height - r_height;
    return h_diff;
}

/***************************
 *  Tree Rotation Methods  *
 ***************************/

AvlNode * rr_rotation(AvlNode * root)
{
    cout << "\trr_rotation @ root: " << root->element << endl;
    /* Root = node we inserted before our current insert
     * rr_tmp = new root; also stores pointer for the left child of root->left.
     * Above info is important as it allows us to easily rebuild the tree.
     */
    AvlNode * rr_tmp = root->left;
    //rr_tmp->right is the left child of our new root, shuffles stuff around appropriately
    root->left = rr_tmp->right;

    /* Make our new root's right the value of the original root
     * because we're moving the original root to the right
     */
    rr_tmp->right = root;

    // Return the new root containing balanced left value and right value pointers
    return update_heights(rr_tmp);
} 


AvlNode * ll_rotation(AvlNode * root)
{
    cout << "\tll_rotation @ root: " << root->element << endl;
    /* Root = node we inserted before our current insert
     * ll_tmp = new root; also stores pointer for the left child of root->left.
     * Above info is important as it allows us to easily rebuild the tree.
     */
    AvlNode * ll_tmp = root->right; 

    // ll_tmp->left is the left child of our new root, shuffles stuff around appropriately
    root->right = ll_tmp->left;

    /* Make our new root's left the value of the original root
     * because we're moving the original root to the left
     */
    ll_tmp->left = root;
    
    // Return the new root containing balanced left value and right value pointers
    return update_heights(ll_tmp);
}

AvlNode * lr_rotation(AvlNode * root)
{   
    cout << "lr_rotation @ root: " << root->element << endl;
    // lr_tmp is our new root; we will rotate values about this
    AvlNode * lr_tmp = root->left;

    /* Now do a ll_rotation to get all nodes on one side (left)
    ll_rotation turns:
    this:
            Node1
          /
       Node2
            \
            Node3

    into this:

              Node1
              /
          Node3
         /
    Node2
    */
    root->left = ll_rotation(lr_tmp);
    /* but we still need to go right to balance it
    return rr_rotation(root) turns:
    this:
              Node1
              /
          Node3
         /
    Node2
    
    into this:

            Node3
          /       \
        Node2    Node1

    So now it is balanced */

    return rr_rotation(root);
}

AvlNode * rl_rotation(AvlNode * root)
{
    cout << "rl_rotation @ root: " << root->element << endl;
    // rl_tmp is our new root. we will balance values around this
    AvlNode * rl_tmp = root->right;
    
    /* Now do a rr_rotation to get all nodes on one side (right)
    rr_rotation turns:
    this:
            Node1
                 \
                  Node2
                 /
              Node3

    into this:

              Node1
                   \
                  Node3
                       \
                        Node2
    */
    root->right = rr_rotation(rl_tmp);
    /* but we still need to go left to balance it
    return ll_rotation(root) turns:
    this:
              Node1
                   \
                    Node3
                         \
                         Node2
    
    into this:

            Node3
          /       \
        Node1    Node3

    So now it is balanced */

    return ll_rotation(root);
}

AvlNode * balance(AvlNode * root)
{
    int h_diff = height_diff(root);
    // If left subtree (R->left) height is greater than right subtree
    // (left h - right h = 2)
    if(h_diff > 1)
    {
        // If left subtree of left subtree height (R->left->left)
        // is greater than height of right subtree of left subtree (R->left->right)
        if(height_diff(root->left) > 0)
            root = rr_rotation(root);
        // Else if left subtree height of left subtree is less than
        // the height of the right subtree of the right subtree
        else if(height_diff(root->left) < 0)
            root = lr_rotation(root);
    }
    // If right subtree height is greater than left subtree
    // (left h - right h = -2)
    else if(h_diff < -1)
    {
        // If left subtree height of right subtree is greater than
        // the height of the right subtree of the right subtree
        if(height_diff(root->right) < 0)
            root = ll_rotation(root);
        else if(height_diff(root->right) > 0)
            root = rl_rotation(root);
    }

    return update_heights(root);
}

AvlNode * update_heights(AvlNode * root)
{
    // Update some heights
    root->height = tree_height(root);
    if(root->left != NULL)
        root->left->height = tree_height(root->left);
    if(root->right != NULL)
        root->right->height = tree_height(root->right);

    return root;
}
///////////////////////////////////

/*
 * Print methods, do not change
 */
void print(AvlNode *root, int level, int type) {

    if (root == NULL) {
        return;
    }

    if (type == IS_ROOT) {
        std::cout << root -> element << " <> height: " << root->height << "\n";
    } 
    
    else {
        for (int i = 1; i < level; i++) {
            std::cout << "| ";
        }
    
        if (type == IS_LEFT) {
            std::cout << "|l_" << root -> element <<  " <> height: " << root->height << "\n";
        } 
    
        else {
            std::cout << "|r_" << root -> element <<  " <> height: " << root->height << "\n";
        }
    }
  
    if (root -> left != NULL) {
        print(root -> left, level + 1, IS_LEFT);
    }
  
    if (root -> right != NULL) {
        print(root -> right, level + 1, IS_RIGHT);
    }

}
void print(AvlNode *root) {
    print(root, 0, IS_ROOT);
}
/*
 * END Print methods, do not change
 */


/* 
 * Main method, do not change
 */
int main(int argc, const char * argv[]) {
    AvlNode *root = NULL;
    std::string filename = argv[1];
    freopen(filename.c_str(), "r", stdin);
    std::string input;
    int data;
    string wait;

    while(std::cin >> input){
        if (input == "insert"){
            std::cin>>data;
            insert(data, root, tree_height(root)+1);
        } 
        else if(input == "delete"){
            std::cin>>data;
            remove(data, root);
        } 
        else if(input == "print"){
            cout << "\n--------------------------\n";
            print(root);
            std::cout << "--------------------------\n\n";
        } 
        else{
            std::cout<<"Data not consistent in file";
            break;
        }
  }

  return 0;
}
/*
 * END main method
 */
