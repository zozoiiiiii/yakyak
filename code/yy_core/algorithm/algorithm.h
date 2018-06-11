/************************************************************************/
/* 
@author:    junliang
@brief:     algorithm
@time:      2017-9-26
*/
/************************************************************************/
#pragma once

#include <assert.h>
#include <stdio.h>
#include <string>

// 单链表
struct Node
{
    int data;
    Node* next;
};

static void Append(Node* root, int data)
{
    Node* tmp = root;
    while(tmp && tmp->next!=NULL)
    {
        tmp=tmp->next;
    }

    Node* pNode = new Node;
    pNode->data=data;
    pNode->next=NULL;
    tmp->next = pNode;
}

static void Delete(Node* root, int data)
{
    // 利用指针的指针来实现
    Node **tmp = &root;
    while ((*tmp) != NULL)
    {
        if ((*tmp)->data == data)
        {
            Node *target = *tmp;
            *tmp = (*tmp)->next;
            delete target;
        }
        else
        {
            tmp = &(*tmp)->next;
        }
    }
}

/************************************************************************/
/*
@brief:  链表
*/
/************************************************************************/
struct ListNode
{
    int elem;
    ListNode* next;
    ListNode* prev;
};
/************************************************************************/
/*
@brief:  二叉树 binary tree
*/
/************************************************************************/
struct BinNode
{
    int elem;
    BinNode* left;
    BinNode* right;
    BinNode* parent;
};
/************************************************************************/
/*
@brief:  哈希函数, 把一串字符串转换成散列值， stl hash map, 查找复杂度O(1)
         参考quake3
         测试发现，VAR_SIZE和插入的数据量，差10倍以上，冲突的比例会比较好
*/
/************************************************************************/
#define VAR_SIZE 1024

struct cvar_t
{
    char key[100];
    char value[100];
    cvar_t* hash_next;
    cvar_t* next;
};

static long generateHashValue( const char *fname )
{
    int        i;
    long    hash;
    char    letter;

    hash = 0;
    i = 0;
    while (fname[i] != '\0')
    {
        letter = tolower(fname[i]);
        hash+=(long)(letter)*(i+119);
        i++;
    }

    hash &= (VAR_SIZE-1);
    return hash;
}

class HashMap
{
public:
    HashMap()
    {
        m_capacity = VAR_SIZE;
        int n=sizeof(m_table);
        memset(m_table, 0, sizeof(m_table));
        m_nodes = new cvar_t[m_capacity];
        m_size = 0;
        m_node_index=0;
        m_node_head=NULL;
    }

    ~HashMap()
    {
        delete []m_nodes;
    }

    cvar_t *get( const char *key)
    {
        assert(strlen(key) < 100);

        long hash;
        cvar_t* kn=find(key);
        if(kn)
            return kn;

        //找不到，则新建
        assert(m_node_index < m_capacity);
        cvar_t* kn_new=&m_nodes[m_node_index++];
        strcpy(kn_new->key, key);

        //新节点插入数组
        hash = generateHashValue(key);
        kn_new->hash_next=m_table[hash];
        m_table[hash]=kn_new;

        //插入链表
        kn_new->next=m_node_head;
        m_node_head=kn_new;

        return kn_new;
    }

    bool isExist(const char* key)
    {
        if(NULL ==find(key))
            return false;

        return true;
    }

    //用hashmap就是为了快速查找海量数据，所以不应该支持remove
    //bool remove(const std::string& key){return false;}


private:
    cvar_t* find(const char* key)
    {
        long hash = generateHashValue(key);
        cvar_t* kn=NULL;
        for(kn=m_table[hash]; kn; kn=kn->hash_next)
        {
            if(strcmp(kn->key, key)==0)
                return kn;
        }

        return NULL;
    }
public:
    cvar_t *m_table[VAR_SIZE];        //std::string->index
    cvar_t *m_nodes;                //新节点从此数组获得新空间
    cvar_t* m_node_head;            //链表头,通过此节点，可以遍历所有在使用的节点
    int m_size;
    int m_capacity;
    int m_node_index;
};


/************************************************************************/
/*
@brief:  平衡二叉树
*/
/************************************************************************/


/************************************************************************/
/*
@brief:  红黑树(自平衡二叉树)
         stl set, multiset, map, multimap, 查找复杂度O(log n)
*/
/************************************************************************/


/************************************************************************/
/* 
@author:    junliang
@brief:     print diamond


    *    
   ***   
  *****  
   ***   
    *    
row:5
column:5


    *    
   ***   
  *****  
 ******* 
  *****  
   ***   
    *    
row:7
column:7

so row equals to column

@time:      2017-6-30
*/
/************************************************************************/

// a表示对角线，即行数和列数
bool PrintDiamond(int a)
{
    // 必须奇数
    if(a%2 == 0)
        return false;

    int mid = (a+1)/2;

    // 上面半个菱形
    for(int i=0; i<mid; i++)
    {
        for(int j=0; j<mid-i-1; j++)
        {
            printf("-");
        }
        for(int k=0; k<2*i+1; k++)
        {
            printf("*");
        }
        printf("\n");
    }

    // 下面半个菱形
    for(int i=mid-2; i>=0; i--)
    {
        for(int j=0; j<mid-i-1; j++)
        {
            printf("-");
        }
        for(int k=0; k<2*i+1; k++)
        {
            printf("*");
        }
        printf("\n");
    }
    return true;
}


/************************************************************************/
/* 
@author:    junliang
@brief:     
@time:      2017-10-19
*/
/************************************************************************/
class Test
{
public:
    // 以下三个接口，复杂度为o(1), 每个节点身上记录以自身节点为栈顶的情况下的最小值
    void push(int n);
    void pop(int n);
    int get_min();
};