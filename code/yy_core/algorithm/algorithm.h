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

// ������
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
    // ����ָ���ָ����ʵ��
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
@brief:  ����
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
@brief:  ������ binary tree
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
@brief:  ��ϣ����, ��һ���ַ���ת����ɢ��ֵ�� stl hash map, ���Ҹ��Ӷ�O(1)
         �ο�quake3
         ���Է��֣�VAR_SIZE�Ͳ��������������10�����ϣ���ͻ�ı�����ȽϺ�
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

        //�Ҳ��������½�
        assert(m_node_index < m_capacity);
        cvar_t* kn_new=&m_nodes[m_node_index++];
        strcpy(kn_new->key, key);

        //�½ڵ��������
        hash = generateHashValue(key);
        kn_new->hash_next=m_table[hash];
        m_table[hash]=kn_new;

        //��������
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

    //��hashmap����Ϊ�˿��ٲ��Һ������ݣ����Բ�Ӧ��֧��remove
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
    cvar_t *m_nodes;                //�½ڵ�Ӵ��������¿ռ�
    cvar_t* m_node_head;            //����ͷ,ͨ���˽ڵ㣬���Ա���������ʹ�õĽڵ�
    int m_size;
    int m_capacity;
    int m_node_index;
};


/************************************************************************/
/*
@brief:  ƽ�������
*/
/************************************************************************/


/************************************************************************/
/*
@brief:  �����(��ƽ�������)
         stl set, multiset, map, multimap, ���Ҹ��Ӷ�O(log n)
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

// a��ʾ�Խ��ߣ�������������
bool PrintDiamond(int a)
{
    // ��������
    if(a%2 == 0)
        return false;

    int mid = (a+1)/2;

    // ����������
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

    // ����������
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
    // ���������ӿڣ����Ӷ�Ϊo(1), ÿ���ڵ����ϼ�¼������ڵ�Ϊջ��������µ���Сֵ
    void push(int n);
    void pop(int n);
    int get_min();
};