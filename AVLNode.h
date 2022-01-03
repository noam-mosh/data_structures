#ifndef AVLNODE_H
#define AVLNODE_H

namespace data_structures {

    template<class T, class S>
    class AVLNode {
//    private:
    public:
        T data;
        S w;
        S w_info;
        AVLNode* father;
        AVLNode* left;
        AVLNode* right;

//    public:
        AVLNode() = default;

        explicit AVLNode(T &value, S &weight_info) :data(value), father(nullptr), left(nullptr), right(nullptr), w(S(weight_info)), w_info(weight_info) {}

        ~AVLNode() = default;

        AVLNode& operator=(const AVLNode<T, S>& node) = default;

//        bool operator==(const AVLNode<T>& node);

        bool operator<(const AVLNode<T,S>& node);

        const T& GetData() const {return data;}

        void SetFather(AVLNode *fatherNode) {father = fatherNode;}

        AVLNode* GetFather() const {return father;}

        void SetLeft(AVLNode *left_Node) {left = left_Node;}

        AVLNode* GetLeft() {return left;}

        void SetRight(AVLNode *rightNode) {right = rightNode;}

        AVLNode* GetRight() const {return right;}

    };

    template<class T, class S>
    bool AVLNode<T, S>::operator<(const AVLNode<T, S> &node) {
        if (data < node.data)
            return true;
        return false;
    }
}
#endif //AVLNODE_H
