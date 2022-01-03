#ifndef WET2_RANKAVLTREE_H
#define WET2_RANKAVLTREE_H

#include "AVLNode.h"
#include "library2.h"
#include <iostream>

namespace data_structures {

    template<class T, class S>
    class RankAVLTree {
    public:
        friend class AVLNode<T, S>;

        AVLNode<T,S>* tree_root;

        int size;

        RankAVLTree() : tree_root(nullptr), size(0) {}

        RankAVLTree(AVLNode<T,S>* root, int n) : tree_root(root), size(n) {}

        ~RankAVLTree();

        RankAVLTree& operator=(const RankAVLTree<T,S>& tree) = default;

        AVLNode<T,S>* Find(AVLNode<T,S> *root, const T &data) const;

        StatusType Insert(T& value, S& w_value);

        void RemoveNode(T& data);

        AVLNode<T,S>* RemoveAVLNode(AVLNode<T,S>* node, T& data);

        void DeleteRecursiveAVLNode(AVLNode<T,S>* node);

        AVLNode<T,S>* InsertAVLNode(AVLNode<T,S>* root, AVLNode<T,S>* inserted);

        int GetHeight(AVLNode<T,S> *root) const;

        int GetBalanceFactor(AVLNode<T,S> *root) const;

        AVLNode<T,S>* GetMax(AVLNode<T,S>* root);

        AVLNode<T,S>* GetMin(AVLNode<T,S>* root);

        AVLNode<T,S>* RotateToLeft(AVLNode<T,S> *root);

        AVLNode<T,S>* RotateToRight(AVLNode<T,S> *root);

        void StoreInOrder(AVLNode<T,S>* root, T* inorder, int n, int* index_ptr);

        S Rank(AVLNode<T,S>* root, int key);

//        AVLNode<T,S>* Select(AVLNode<T,S>* root, int k);

        AVLNode<T,S>* SelectMinBigger(AVLNode<T,S>* root, int k);
    };

    template<class T, class S>
    S RankAVLTree<T,S>::Rank(AVLNode<T,S>* root, int key)
    {
        S r{};
        while (root->data != key)
        {
            if (root->data < key) {
                r += root->left->w + root->w_info;
                root = root->right;
            }
            if (root->data > key)
                root = root->left;
        }
        if (root->left == nullptr)
            r += root->w_info;
        else
            r += root->left->w + root->w_info;
        return r;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::SelectMinBigger(AVLNode<T,S>* root, int k)
    {
        if (root->left && (root->left->w == ((root->w_info - k)*(-1))))
            return root;
        if (root->left && (root->left->w < ((root->w_info - k)*(-1))))
        {
            if (root->right == nullptr)
                return nullptr;
            return SelectMinBigger(root->right, ((root->left->w + root->w_info) - k)*(-1));
        }
        else if (root->left && root->left->w < k)
            return root;
        else if (root->left)
            return SelectMinBigger(root->left, k);
        return root;
    }

    template<class T, class S>
    T* MergeArrays(T* array1, T* array2, int n1, int n2);

    template<class T, class S>
    RankAVLTree<T,S>::~RankAVLTree() {
        if (tree_root) {
            DeleteRecursiveAVLNode(tree_root);
        }
    }

    template<class T, class S>
    void RankAVLTree<T,S>::DeleteRecursiveAVLNode(AVLNode<T,S> *node) {
        if (node) {
            DeleteRecursiveAVLNode(node->GetLeft());
            DeleteRecursiveAVLNode(node->GetRight());
            delete node;
            size--;
        }
    }

    template<class T, class S>
    StatusType RankAVLTree<T,S>::Insert(T& data_value, S& w_value) {
        if (Find(tree_root, data_value))
            return INVALID_INPUT;
        auto* new_node = new AVLNode<T,S>(data_value, w_value);
        if (!new_node)
            return ALLOCATION_ERROR;
        if (!tree_root)
            tree_root = new_node;
        else
            InsertAVLNode(tree_root, new_node);
        size++;
        return SUCCESS;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::InsertAVLNode(AVLNode<T,S>* root, AVLNode<T,S>* inserted) {
        if (root->GetData() == inserted->GetData())
            return nullptr;
        if (root->GetData() > inserted->GetData()) {
            if (root->GetLeft())
                InsertAVLNode(root->GetLeft(), inserted);
            else {
                root->SetLeft(inserted);
                inserted->SetFather(root);
            }
        } else {
            if (root->GetRight())
                InsertAVLNode(root->GetRight(), inserted);
            else {
                root->SetRight(inserted);
                inserted->SetFather(root);
            }
        }

        // Update weight
        root->w = root->w_info;
        if (root->left)
            root->w += root->left->w;
        if (root->right)
            root->w += root->right->w;

        int balance = GetBalanceFactor(root);

        // Left Left Rotation
        if (balance > 1 && inserted->GetData() < (root->GetLeft())->GetData())
            return RotateToRight(root);
        // Right Right Rotation
        if (balance < -1 && inserted->GetData() > (root->GetRight())->GetData())
            return RotateToLeft(root);
        // Left Right Rotation
        if (balance > 1 && inserted->GetData() > (root->GetLeft())->GetData())
        {
            root->SetLeft(RotateToLeft(root->GetLeft()));
            return RotateToRight(root);
        }
        // Right Left Rotation
        if (balance < -1 && inserted->GetData() < (root->GetRight())->GetData())
        {
            root->SetRight(RotateToRight(root->GetRight()));
            return RotateToLeft(root);
        }
        return root;
    }

    template<class T, class S>
    AVLNode<T,S> *RankAVLTree<T,S>::Find(AVLNode<T,S>* root, const T& data) const {
        if (root) {
            if (root->GetData() == data)
                return root;
            else if (data < root->GetData())
                return Find(root->GetLeft(), data);
            else
                return Find(root->GetRight(), data);
        }
        return nullptr;
    }

    template<class T, class S>
    int RankAVLTree<T,S>::GetHeight(AVLNode<T,S>* root) const {
        int height = 0;
        if (root) {
            int leftHeight = GetHeight(root->GetLeft());
            int rightHeight = GetHeight(root->GetRight());
            height = 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
        }
        return height;
    }

    template<class T, class S>
    int RankAVLTree<T,S>::GetBalanceFactor(AVLNode<T,S>* root) const {
        int balance = 0;
        if (root)
            balance = GetHeight(root->GetLeft()) - GetHeight(root->GetRight());
        return balance;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::RotateToLeft(AVLNode<T,S>* root) {
        AVLNode<T,S>* newRoot = root->GetRight();
        root->SetRight(newRoot->GetLeft());
        if (newRoot->GetLeft())
            newRoot->GetLeft()->SetFather(root);
        newRoot->SetLeft(root);

        if (root->GetFather() == nullptr) {
            tree_root = newRoot;
            newRoot->SetFather(nullptr);
        } else {
            if (root == root->GetFather()->GetLeft()) {
                //node is the left son of his father
                root->GetFather()->SetLeft(newRoot);
            } else {
                //node is the right son of his father
                root->GetFather()->SetRight(newRoot);
            }
            newRoot->SetFather(root->GetFather());
        }
        root->SetFather(newRoot);

        root->w = root->w_info;
        if (root->left)
            root->w = root->left->w;
        if (root->right)
            root->w += root->right->w;

        newRoot->w = newRoot->w_info;
        if (newRoot->left)
            newRoot->w += newRoot->left->w;
        if (newRoot->right)
            newRoot->w += newRoot->right->w;

        return newRoot;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::RotateToRight(AVLNode<T,S>* root) {
        AVLNode<T,S>* newRoot = root->GetLeft();
        root->SetLeft(newRoot->GetRight());
        if (newRoot->GetRight())
            newRoot->GetRight()->SetFather(root);
        newRoot->SetRight(root);

        if (root->GetFather() == nullptr) {
            tree_root = newRoot;
            newRoot->SetFather(nullptr);
        } else {
            if (root == root->GetFather()->GetLeft()) {
                //node is the left son of his father
                root->GetFather()->SetLeft(newRoot);
            } else {
                //node is the right son of his father
                root->GetFather()->SetRight(newRoot);
            }
            newRoot->SetFather(root->GetFather());
        }
        root->SetFather(newRoot);
        // update weight
        root->w = root->w_info;
        if (root->left)
            root->w += root->left->w;
        if (root->right)
            root->w += root->right->w;

        newRoot->w = newRoot->w_info;
        if (newRoot->left)
            newRoot->w += newRoot->left->w;
        if (newRoot->right)
            newRoot->w += newRoot->right->w;

        return newRoot;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::GetMax(AVLNode<T,S>* root){
        AVLNode<T,S>* current = root;
        while (current->GetRight() != nullptr)
            current = current->GetRight();
        return current;
    }

    template<class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::GetMin(AVLNode<T,S>* root)
    {
        AVLNode<T,S>* current = root;
        while (current->GetLeft() != nullptr)
            current = current->GetLeft();
        return current;
    }

    template<class T, class S>
    void RankAVLTree<T,S>::StoreInOrder(AVLNode<T,S>* root, T* inorder, int n, int* index_ptr)
    {
        if (root == nullptr || inorder == nullptr || *index_ptr == n)
            return;
        StoreInOrder(root->GetLeft(), inorder, n, index_ptr);
        if (*index_ptr == n)
            return;
        inorder[*index_ptr] = root->GetData();
        (*index_ptr)++; // increase index for next entry
        StoreInOrder(root->GetRight(), inorder, n, index_ptr);
    }

    // A utility function that merges two sorted arrays into one
    template<class T, class S>
    T* MergeArrays(T* array1, T* array2, int n1, int n2)
    {
        T* mergedArray = new T[n1 + n2]();
        int i = 0, j = 0, k = 0;
        while (i < n1 && j < n2)
        {
            if (array1[i] < array2[j])
                mergedArray[k++] = array1[i++];
            else
                mergedArray[k++] = array2[j++];
        }
        // If array1 is bigger
        while (i < n1)
            mergedArray[k++] = array1[i++];
        // If array2 is bigger
        while (j < n2)
            mergedArray[k++] = array2[j++];
        return mergedArray;
    }

    template <class T, class S>
    AVLNode<T,S>* RankAVLTree<T,S>::RemoveAVLNode(AVLNode<T,S>* node, T& data)
    {
        if (node == nullptr)
            return node;
        if (data < node->GetData())
            node->SetLeft(RemoveAVLNode(node->GetLeft(), data));
        else if(data > node->GetData())
            node->SetRight(RemoveAVLNode(node->GetRight(), data));
        else
        {
            // node with only one son or no son
            if((node->GetLeft() == nullptr) || (node->GetRight() == nullptr))
            {
                AVLNode<T,S>* son = node->GetLeft() ? node->GetLeft() : node->GetRight();
                // No child case
                if (son == nullptr)
                {
                    delete node;
                    node = nullptr;
                }
                    // One son case
                else
                {
                    if (node->GetFather() && node->GetFather()->GetRight() == node)
                        node->GetFather()->SetRight(son);
                    else if (node->GetFather() && node->GetFather()->GetLeft() == node)
                        node->GetFather()->SetLeft(son);
                    son->SetFather(node->GetFather());
                    // Delete
                    if (data == tree_root->data)
                        tree_root = son;
                    delete node;
                    node = nullptr;
                    return son;
                }
            }
            else
            {
                // node with two sons:
                AVLNode<T,S>* minimal = GetMin(node->GetRight());
                if (minimal == nullptr)
                    return nullptr;
                // connect min
                AVLNode<T,S> *min_father = minimal->GetFather();
                AVLNode<T,S> *min_right = minimal->GetRight();
                // node is root

                if (node->GetFather() && node->GetFather()->GetRight() == node)
                    node->GetFather()->SetRight(minimal);
                else if (node->GetFather() && node->GetFather()->GetLeft() == node)
                    node->GetFather()->SetLeft(minimal);
                if (data == tree_root->data)
                    tree_root = minimal;
                minimal->SetLeft(node->GetLeft());
                node->GetLeft()->SetFather(minimal);
                minimal->SetFather(node->GetFather());
                if (node->GetRight() == minimal)
                {
                    minimal->SetRight(node);
                    node->SetRight(min_right);
                    if (min_right)
                        min_right->SetFather(node);
                    node->SetFather(minimal);
                    node->SetLeft(nullptr);
                }
                else {
                    minimal->SetRight(node->GetRight());
                    node->GetRight()->SetFather(minimal);
                    // connect node
                    if (min_father->GetRight() == minimal)
                        min_father->SetRight(node);
                    else if (min_father->GetLeft() == minimal)
                        min_father->SetLeft(node);
                    node->SetLeft(nullptr);
                    node->SetRight(min_right);
                    node->SetFather(min_father);

                    if (min_right)
                        min_right->SetFather(node);
                }
                minimal->SetRight(RemoveAVLNode(minimal->GetRight(), node->data));
                node = minimal;
            }
        }
        // If the tree had only one node
        if (node == nullptr)
            return node;

        // Update weight
        node->w = node->w_info;
        if (node->left)
            node->w += node->left->w;
        if (node->right)
            node->w += node->right->w;

        int balance = GetBalanceFactor(node);

        // Left Left Rotation
        if (balance > 1 && GetBalanceFactor(node->GetLeft()) >= 0)
            return RotateToRight(node);
        // Left Right Rotation
        if (balance > 1 && GetBalanceFactor(node->GetLeft()) < 0)
        {
            node->SetLeft(RotateToLeft(node->GetLeft()));
            return RotateToRight(node);
        }
        // Right Right Rotation
        if (balance < -1 && GetBalanceFactor(node->GetRight()) <= 0)
            return RotateToLeft(node);
        // Right Left Rotation
        if (balance < -1 && GetBalanceFactor(node->GetRight()) > 0)
        {
            node->SetRight(RotateToRight(node->GetRight()));
            return RotateToLeft(node);
        }
        // update weight
        return node;
    }

    template <class T, class S>
    void RankAVLTree<T,S>::RemoveNode(T& data)
    {
        if(Find(tree_root, data))
        {
            if (size == 1) {
                delete tree_root;
                tree_root = nullptr;
            }
            else
                RemoveAVLNode(tree_root, data);
            size--;
        }
    }

    template <class T, class S>
    AVLNode<T,S>* BuildAVLTreeFromSortedArray(T* array, int start, int end, AVLNode<T,S>* root)
    {
        if (start > end)
            return nullptr;
        int mid = (start + end)/2;
        auto* node = new AVLNode<T,S>(array[mid]);
        node->SetFather(root);
        node->SetLeft(BuildAVLTreeFromSortedArray(array, start, mid-1, node));
        node->SetRight(BuildAVLTreeFromSortedArray(array, mid+1, end, node));
        return node;
    }

    template <class T, class S>
    RankAVLTree<T,S> MergeTwoAVLTrees(RankAVLTree<T,S>* tree1, RankAVLTree<T,S>* tree2){
        int size1 = tree1->size, size2 = tree2->size;
        T *array1 = new T[size1]();
        T *array2 = new T[size2]();
        int index1 = 0, index2 = 0;
        tree1->StoreInOrder(tree1->tree_root, array1, size1, &index1);
        tree2->StoreInOrder(tree2->tree_root, array2, size2, &index2);
        T* mergedArray = MergeArrays(array1, array2, size1, size2);
        AVLNode<T,S>* root = nullptr;
        RankAVLTree<T,S> mergedTree(BuildAVLTreeFromSortedArray(mergedArray, 0, (size1 + size2 - 1), root), (size1 + size2));
        delete[] array1;
        delete[] array2;
        delete[] mergedArray;
        return mergedTree;
    } }

#endif //WET2_RANKAVLTREE_H
