#include <vector>
#include <queue>

using namespace std;

template <class W>
struct HuffmanTreeNode
{
    HuffmanTreeNode<W> *_left;
    HuffmanTreeNode<W> *_right;
    HuffmanTreeNode<W> *_parent;
    W _weight;

    HuffmanTreeNode(const W &w = W())
        : _left(nullptr), _right(nullptr), _parent(nullptr), _weight(w)
    {
    }
};

template <class W>
class HuffmanTree
{
    typedef HuffmanTreeNode<W> Node;
    class Compare
    {
    public:
        bool operator()(const Node *x, const Node *y) const
        {
            return x->_weight > y->_weight;
        }
    };

public:
    HuffmanTree(const vector<W> &vw, const W &invalid)
        : _root(nullptr)
    {
        // 构建优先级队列
        priority_queue<Node *, vector<Node *>, Compare> q;
        for (auto &e : vw)
        {
            if (invalid != e)
                q.push(new Node(e));
        }

        while (q.size() > 1)
        {
            Node *left = q.top();
            q.pop();
            Node *right = q.top();
            q.pop();

            Node *parent = new Node(left->_weight + right->_weight);
            parent->_left = left;
            left->_parent = parent;

            parent->_right = right;
            right->_parent = parent;

            q.push(parent);
        }

        _root = q.top();
    }

    ~HuffmanTree()
    {
        destroyTree(_root);
    }

    Node *GetRoot()
    {
        return _root;
    }

private:
    void destroyTree(Node *node)
    {
        if (node->_left != nullptr)
            destroyTree(node->_left);
        if (node->_right != nullptr)
            destroyTree(node->_right);
        delete node;
        node = nullptr;
    }

    Node *_root;
};