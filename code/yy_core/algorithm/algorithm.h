/************************************************************************/
/* 
@author:    junliang
@brief:     algorithm





Cpp application in memory layout(4GB in win32)
_______________________________________________high address
Cmd line argument
_______________________________________________
Stack
------------------------------------------------------
  |
......
_______________________________________________
Shared libraries(malloc.o(lib*.so)                     dynamic load library
_______________________________________________
......

  |
------------------------------------------------------
Heap(malloc, calloc, new)
_______________________________________________
Data(Global/static, intialized and unitialize, int y=100)
_______________________________________________
Text(compiled code like main.o, file.o,  a.out)       malloc.o, static link library
_______________________________________________low adderss





----------------------------------------------------------------------------------------
Stack


Int add(int a, int b)
{Int c;
c=a+b;
Return c}

Call Add(4,8) function

_______________________________________________low

Local data               0xc
_______________________________________________
EBP
_______________________________________________
Return address       [ebp]+4
_______________________________________________
4              [ebp]+8
8              [ebp]+12
_______________________________________________high



traversal: 遍历
bfs: breadth first search, 广度优先遍历
dfs: deep first search, 深度优先遍历
@time:      2017-9-26
*/
/************************************************************************/
#pragma once

#include <assert.h>
#include <stdio.h>
#include <string>
#include <vector>


namespace AlgorithmNS
{
	class MyBase
	{
	public:
		MyBase();
		~MyBase();

	private:
		const int SIZE = 100;
		//char m_buf[SIZE];
	};

	class MyA : public MyBase
	{
	public:
	};




	/*

	ListNS::Node* node1 = new ListNS::Node;
	ListNS::Node* node2 = new ListNS::Node;
	ListNS::Node* node3 = new ListNS::Node;

	node1->data = 1;
	node1->next = node2;
	node2->data = 2;
	node2->next = node3;
	node3->data = 3;
	node3->next = nullptr;

	ListNS::Node* pNode = ListNS::Delete(node2, 2);
	*/
	struct Node
	{
		int data;
		Node* next;
	};

	static void Append(Node* root, int data);
	static Node* deleteNode(Node* root, int value);
	static Node* Delete(Node* pNode, int data);


	// list
	// struct ListNode
	// {
	//     int elem;
	//     ListNode* next;
	//     ListNode* prev;
	// };
	/************************************************************************/
	/*
	@brief:  binary tree
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
	@brief:  hash map, complexity: O(1)
			 consider quake3
			 VAR_SIZE * 10 < inserted elements will be better.
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

	static long generateHashValue(const char *fname);
	class HashMap
	{
	public:
		HashMap();
		~HashMap();
		cvar_t *get(const char *key);
		bool isExist(const char* key);

		//hashmap is good at finding, so should not support remove
		//bool remove(const std::string& key){return false;}


	private:
		cvar_t* find(const char* key);
	public:
		cvar_t *m_table[VAR_SIZE];        //std::string->index
		cvar_t *m_nodes;
		cvar_t* m_node_head;
		int m_size;
		int m_capacity;
		int m_node_index;
	};


	/************************************************************************/
	/*
	@brief:  balance binary tree
	*/
	/************************************************************************/


	/************************************************************************/
	/*
	@brief:  read black tree(balance self binary tree)
			 stl set, multiset, map, multimap, complex:O(log n)
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

	so row equals to column

	@time:      2017-6-30
	*/
	/************************************************************************/

	// a = row number = column number
	static bool PrintDiamond(int a);


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
		// complex: O(1), 每个节点身上记录以自身节点为栈顶的情况下的最小值
		//void push(int n);
		//void pop(int n);
		//int get_min();
	};




	// 1000w int, how can i find element exsit. 如何判断一个元素在亿级数据中是否存在？, Bloom Filter
	// sure the value not exist. the value maybe exist.  used in cache, database
	class BigDataFinder
	{
	public:
		BigDataFinder();
		void load(const std::vector<int>& nums);
		bool isNotExist(int nValue);
		bool maybeExist(int nValue);

	private:
		int* m_data;
	};

}