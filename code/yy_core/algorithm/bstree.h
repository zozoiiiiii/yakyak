/************************************************************************/
/*
@author:  junliang
@brief:   bstree
@time:    20130401
*/
/************************************************************************/
#ifndef _YYBINARY_SEARCH_TREE_H_
#define _YYBINARY_SEARCH_TREE_H_


template<class KEY>
class less
{
public:
    bool operator()(const KEY& key1, const KEY key2) const
    {
        return (key1 < key2);
    }
};


// right < left
template<class KEY, class VALUE>
struct CBsTreeNode
{
    KEY key;
    VALUE val;
    CBsTreeNode<KEY, VALUE>* parent;
    CBsTreeNode<KEY, VALUE>* left;
    CBsTreeNode<KEY, VALUE>* right;

    CBsTreeNode():parent(NULL), left(NULL), right(NULL){}
    CBsTreeNode(const KEY& pKey, const VALUE& pVal):key(pKey), val(pVal), parent(NULL), left(NULL), right(NULL){}
    CBsTreeNode(const KEY& pKey, const VALUE& pVal, CBsTreeNode* pParent):key(pKey), val(pVal), parent(pParent), left(NULL), right(NULL){}

    void clear()
    {
        if(left)
            left->clear();
        if(right)
            right->clear();

        delete this;
    }
};


//key struct should reimplement <, ==
template<class KEY, class VALUE, class CmpFunc=less<KEY>>
class CBsTree
{
private:

public:
    CBsTree();
    ~CBsTree();

    void clear();

    void insert(const KEY& pKey, const VALUE& pVal);
    void remove(const KEY& pKey);

	// if not exist, will insert default vaule.
    VALUE& get(const KEY& pKey);
    CBsTreeNode<KEY, VALUE>* searchNode(const KEY& pKey);
private:
    CBsTreeNode<KEY, VALUE>* m_pRoot;
};

template<class KEY, class VALUE, class CmpFunc>
CBsTree<KEY, VALUE, CmpFunc>::CBsTree()
:m_pRoot(NULL)
{}

template<class KEY, class VALUE, class CmpFunc>
CBsTree<KEY, VALUE, CmpFunc>::~CBsTree()
{
    clear();
}

template<class KEY, class VALUE, class CmpFunc>
void CBsTree<KEY, VALUE, CmpFunc>::clear()
{
    if(m_pRoot)
        m_pRoot->clear();
}

template<class KEY, class VALUE, class CmpFunc>
void CBsTree<KEY, VALUE, CmpFunc>::insert(const KEY& pKey, const VALUE& pVal)
{
    if(!m_pRoot)
    {
        m_pRoot=new CBsTreeNode<KEY, VALUE>(pKey, pVal);
        return;
    }

    CBsTreeNode<KEY, VALUE>* tmp=m_pRoot;
    while(tmp)
    {
        if(CmpFunc()(tmp->key, pKey))
        {
            if(!tmp->left)
            {
                tmp->left=new CBsTreeNode<KEY, VALUE>(pKey, pVal, tmp);
                break;
            }

            tmp=tmp->left;
        }
        else
        {
            if(!tmp->right)
            {
                tmp->right=new CBsTreeNode<KEY, VALUE>(pKey, pVal, tmp);
                break;
            }

            tmp=tmp->right;
        }
    }
}

template<class KEY, class VALUE, class CmpFunc>
void CBsTree<KEY, VALUE, CmpFunc>::remove(const KEY& pKey)
{
    CBsTreeNode<KEY, VALUE>* parent=NULL;
    CBsTreeNode<KEY, VALUE>* tmp=search(pKey);
    if(tmp)
    {
        parent=tmp->parent;
        if(!parent)
        {
            m_pRoot=NULL;
            delete tmp;
            return;
        }

        parent->left = tmp->left;
        parent->right = tmp->right;

        if(tmp->left->parent)
            tmp->left->parent=parent;

        if(tmp->right->parent)
            tmp->right->parent=parent;

        delete tmp;
    }
}

template<class KEY, class VALUE, class CmpFunc>
CBsTreeNode<KEY, VALUE>* CBsTree<KEY, VALUE, CmpFunc>::searchNode(const KEY& pKey)
{
    CBsTreeNode<KEY, VALUE>* tmp=m_pRoot;
    while(tmp)
    {
        if(tmp->key == pKey)
            return tmp;

        if(CmpFunc()(tmp->key, pKey))
            tmp=tmp->left;
        else
            tmp=tmp->right;
    }

    return NULL;
}

template<class KEY, class VALUE, class CmpFunc>
VALUE& CBsTree<KEY, VALUE, CmpFunc>::get(const KEY& pKey)
{
    CBsTreeNode<KEY, VALUE>* tmp=searchNode(pKey);
    if(tmp)
        return tmp->val;

    VALUE v;
    insert(pKey, v);
    return v;
}

#endif