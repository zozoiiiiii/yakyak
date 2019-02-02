/************************************************************************/
/* 
@author:    junliang
@brief:     leetcode
@time:      2017-9-26
*/
/************************************************************************/
#pragma once

#include <assert.h>
#include <stdio.h>
#include <string>

namespace ListNS
{
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

	static void Append(Node* root, int data)
	{
		Node* tmp = root;
		while (tmp && tmp->next != NULL)
		{
			tmp = tmp->next;
		}

		Node* pNode = new Node;
		pNode->data = data;
		pNode->next = NULL;
		tmp->next = pNode;
	}

	static Node* deleteNode(Node* root, int value)
	{

		Node* pNode = root;
		if (!pNode)
		{
			return nullptr;
		}

		Node * pCurrent = pNode;
		Node * pPrev = nullptr;

		while (pCurrent && pCurrent->data != value)
		{
			pPrev = pCurrent;
			pCurrent = pCurrent->next;
		}

		if (!pCurrent)
		{
			// not found
			return pNode;
		}

		if (pCurrent == pNode)
		{
			// matches with the first item
			return pNode->next;
		}
		else
		{
			pPrev->next = pCurrent->next;
		}

		return pNode;
	}

	static Node* Delete(Node* pNode, int data)
	{
		Node **tmp = &pNode;
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

		return pNode;
	}

}

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

		// can not find, then create
        assert(m_node_index < m_capacity);
        cvar_t* kn_new=&m_nodes[m_node_index++];
        strcpy(kn_new->key, key);

		// insert the new node.
        hash = generateHashValue(key);
        kn_new->hash_next=m_table[hash];
        m_table[hash]=kn_new;

        // insert to list
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

    //hashmap is good at finding, so should not support remove
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

// a = row number = column number
static bool PrintDiamond(int a)
{
    if(a%2 == 0)
        return false;

    int mid = (a+1)/2;

    // half up
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

    // half down
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
    // complex: O(1), 每个节点身上记录以自身节点为栈顶的情况下的最小值
    void push(int n);
    void pop(int n);
    int get_min();
};




/************************************************************************/
/*
@author:  junliang
@brief:   from leetcoder
@time:    2018/11/30
*/
/************************************************************************/
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "../math/inc/yy_math_func.h"
/*
	std::vector<int> numbers;
	numbers.push_back(2);
	numbers.push_back(7);
	numbers.push_back(11);
	numbers.push_back(15);
	std::vector<int> result = twoSum(numbers, 9);
*/
// 1
static std::vector<int> twoSum(const std::vector<int>& numbers, int target)
{
	std::vector<int> result;
	std::map<int, int> cache;	// value, index
	int nSize = numbers.size();
	for (int i = 0; i < nSize; i++)
	{
		int value = numbers[i];
		int leaveValue = target - value;
		auto itor = cache.find(leaveValue);
		if (itor != cache.end())
		{
			int leaveIndex = itor->second;

			int minIndex = min(i, leaveIndex);
			int maxIndex = max(i, leaveIndex);
			result.push_back( minIndex);
			result.push_back(maxIndex);
			return result;
		}

		// even if same value exist, will be okay.
		cache[value] = i;
	}

	return result;
}

struct ListNode
{
	int value;
	ListNode* next;

	ListNode() :value(0),next(nullptr) {}
	ListNode(int inputValue, ListNode* inputNext) :value(inputValue), next(inputNext) {}
};

// 2
namespace addTwoNumbersNS
{


	static ListNode* addTwoNumbers(const ListNode* n1, const ListNode* n2)
	{
		int sum = 0;
		ListNode* pHeader = nullptr;
		ListNode* pLastNode = nullptr;
		while (n1 || n2)
		{
			if (n1)
			{
				sum += n1->value;
				n1 = n1->next;
			}

			if (n2)
			{
				sum += n2->value;
				n2 = n2->next;
			}

			ListNode* pNode = new ListNode;
			pNode->value = sum%10;
			sum = sum / 10;
			if (pLastNode)
				pLastNode->next = pNode;
			else
				pHeader = pNode;

			pLastNode = pNode;
		}

		return pHeader;
	}
}


namespace sqrtNS
{
	// twofen

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

	// bisection method
	static double bisectionMethod(double in)
	{
		if (in <= 0)
			return 0;

		double low = 0.0f;
		double hight = in;
		double target = 0.0f;
		double mid = 0.0f;
		while (abs(in - target * target) > 0.001)
		{
			target = (low + hight) / 2.0f;
			if (target*target>in)
			{
				hight = target;
			}
			else if(target*target<in)
			{
				low = target;
			}
		}

		return target;
	}
}


// 4
namespace midianNS
{
	/*

	std::vector<int> a1;
	std::vector<int> a2;
	a1.push_back(1);
	a1.push_back(3);
	a1.push_back(4);
	a1.push_back(5);
	a1.push_back(7);

	a2.push_back(1);
	a2.push_back(5);
	a2.push_back(6);
	a2.push_back(7);
	a2.push_back(8);


	float m = midianNS::getMedianHard(a1, a2);
	*/
	// O(1)
	static float getMedianFromOneArray(const std::vector<int>& nums)
	{
		int len = nums.size();
		int half = len / 2;
		if (len % 2 == 0)
			return (nums[len / 2] + nums[len / 2 + 1]) / 2.0f; 
		else
			return nums[len / 2 + 1];
	}

	//O(nLogn)
	static float getMedianSimple(const std::vector<int>& nums1, const std::vector<int>& nums2)
	{
		std::vector<int> total = nums1;
		total.insert(total.end(), nums2.begin(), nums2.end());

		std::sort(total.begin(), total.end());

		int len = total.size();
		if (len % 2 == 0)
			return (total[len / 2] + total[len / 2 + 1]) / 2.0f;
		else
			return total[len / 2 + 1];		
	}



	//if (aMid < bMid) Keep[aMid + 1 ... n] and [bLeft ... m]
	//else Keep[bMid + 1 ... m] and [aLeft ... n]

	// a1,a2,m1,a4,a5; b1,b2,m2,b5,b5
	// if(m1 < m2) sort(a+b) = a1+a2 + ... + m1 + ... + m2 + b4 + b5 + ..., then can remove a1,a2
	static float getKth(const std::vector<int>& nums1, const std::vector<int>& nums2, int k)
	{
		int m = nums1.size();
		int n = nums2.size();
		if (m > n)	// makesure m<n
			return getKth(nums2, nums1, k);
		if (m == 0)
			return nums2[k - 1];
		if (k == 1)
			return min(nums1[0], nums2[0]);

		int i = min(m, k / 2);
		int j = min(n, k / 2);
		if (nums1[i - 1] < nums2[j - 1])
		{
			std::vector<int> tmp;
			tmp.insert(tmp.end(), nums1.begin() + i, nums1.end());
			return getKth(tmp, nums2, k - i);
		}
		else
		{
			std::vector<int> tmp;
			tmp.insert(tmp.end(), nums2.begin() + j, nums2.end());
			return getKth(nums1, tmp, k - j);
		}
	}

	// O(log(m+n), array sorted up.
	static float getMedianHard(const std::vector<int>& nums1, const std::vector<int>& nums2)
	{
 		int m = nums1.size();
 		int n = nums2.size();
		int total = m + n;
		int half = total / 2;

		if (total % 2 != 0)
			return getKth(nums1, nums2, half + 1);
		else
		return (getKth(nums1, nums2, half) + getKth(nums1, nums2, half+1)) / 2.0f;
	}

}

// 5 longest palindromic substring
namespace LPSNS
{
	// palindromic string, bob, level, noon, you can read fron left to right, or right to left.
	// given a string s, find the longest palindromic substring in s, you may assume that the maximum length of s is 1000, and there
	// exists one unique longest palindromic substring.

	static void searchPalindrome(string s, int left, int right, int& start, int& maxLen)
	{
		while (left >= 0 && right < s.size() && s[left] == s[right])
		{
			--left; ++right;
		}

		// check wehther is longest
		if (maxLen < right - left - 1)
		{
			start = left + 1;
			maxLen = right - left - 1;
		}
	}

	static string longestPalindrome(string s)
	{
		if (s.size() < 2)
			return s;

		int n = s.size(), maxLen = 0, start = 0;
		for (int i = 0; i < n - 1; ++i)
		{
			// bob, expand from i
			searchPalindrome(s, i, i, start, maxLen);

			// noon, expand from i and i+1
			searchPalindrome(s, i, i + 1, start, maxLen);
		}
		return s.substr(start, maxLen);
	}
}

// 8 atoi
namespace atoiNS
{
	// "123", consider +, -
	static int atoi(const char* buf)
	{
		int sign = 1;
		int nStart = 0;
		if (buf[0] == '+' || buf[0] == '-')
		{
			sign = 1 - 2 * (buf[0] == '+');
			nStart = 1;
		}

		int result = 0;
		for (int i = nStart; buf[i] != '\0'; i++)
		{
			int digit = buf[i] - '0';
			if (digit < 0 || digit>9)
				return 0;

			result = result * 10 + digit;
		}

		return result * sign;
	}
}

// 13 roman to integer
namespace RTINS
{
	// given a roman numeral, convert it to an interger.
	/*
SYMBOL       VALUE
I             1
IV            4
V             5
IX            9
X             10
XL            40
L             50
XC            90
C             100
CD            400
D             500
CM            900
M             1000
	*/

	// This function returns value of a Roman symbol 
	static int value(char r)
	{
		if (r == 'I')
			return 1;
		if (r == 'V')
			return 5;
		if (r == 'X')
			return 10;
		if (r == 'L')
			return 50;
		if (r == 'C')
			return 100;
		if (r == 'D')
			return 500;
		if (r == 'M')
			return 1000;

		return -1;
	}

	// Returns decimal value of roman numaral 
	static int romanToDecimal(string &str)
	{
		// Initialize result 
		int res = 0;

		// Traverse given input 
		for (int i = 0; i < str.length(); i++)
		{
			// Getting value of symbol s[i] 
			int s1 = value(str[i]);

			if (i + 1 < str.length())
			{
				// Getting value of symbol s[i+1] 
				int s2 = value(str[i + 1]);

				// Comparing both values 
				if (s1 >= s2)
				{
					// Value of current symbol is greater 
					// or equal to the next symbol 
					res = res + s1;
				}
				else
				{
					res = res + s2 - s1;
					i++; // Value of current symbol is 
						 // less than the next symbol 
				}
			}
			else
			{
				res = res + s1;
				i++;
			}
		}
		return res;
	}
}

// 15 3sum
namespace ThreeSUMNS
{
	//For example, given array S = { -1 0 1 2 - 1 - 4 }, A solution set is :
	//(-1, 0, 1)
	//(-1, -1, 2)
	/*

	std::vector<int> s;
	s.push_back(-1);
	s.push_back(0);
	s.push_back(1);
	s.push_back(2);
	s.push_back(-1);
	s.push_back(-4);
	std::vector<ThreeSUMNS::ThreeInfo> result = ThreeSUMNS::threeSum(s);
	*/

	struct ThreeInfo
	{
		int a;
		int b;
		int c;

		ThreeInfo(int nA, int nB, int nC) :a(nA), b(nB), c(nC) {}
	};

	static std::vector<ThreeInfo> threeSum(std::vector<int> nums)
	{
		std::vector<ThreeInfo> result;

		// sort
		std::sort(nums.begin(), nums.end());

		int nSize = nums.size();
		for (int i = 0; i < nSize - 2; i++)
		{
			// skip the duplicate triplets
			if(i>0&&nums[i] == nums[i-1])
				continue;

			int a = nums[i];
			int start = i + 1;
			int end = nSize - 1;

			while (start < end)
			{
				int b = nums[start];
				int c = nums[end];
				if (a + b + c == 0)
				{
					ThreeInfo ti(a, b, c);
					result.push_back(ti);

					// skip the duplicate triplets
					while (start < end && nums[start] == nums[start + 1])start++;
					while (start < end&&nums[end] == nums[end - 1])end--;

					start++;
					end--;
				}
				else if (a + b + c > 0)
					end--;
				else
					start++;
			}
		}

		return result;
	}
}

// 20, valid parentheses
#include <stack>
namespace SSNS
{
	// "()[]" true,  "(]) false 
	//bool bb = SSNS::isValid("{}[]()");
	static bool isValid(const std::string& s)
	{
		std::stack<int> container;
		int nSize = s.length();
		for (int i = 0; i < nSize; i++)
		{
			char c = s.at(i);
			if (c == '[' || c == '{' || c == '(')
				container.push(c);
			else if (c == ']' && container.top() == '[')
				container.pop();
			else if (c == '}' && container.top() == '{')
				container.pop();
			else if (c == ')' && container.top() == '(')
				container.pop();
			else
				return false;			
		}

		return container.empty();
	}
}




NS_YY_BEGIN
// 21, merge two sorted lists

/*

TwoOneNS::ListNode a4(4, nullptr);
TwoOneNS::ListNode a3(3, &a4);
TwoOneNS::ListNode a2(2, &a3);
TwoOneNS::ListNode a1(1, &a2);


TwoOneNS::ListNode b4(7, nullptr);
TwoOneNS::ListNode b3(4, &b4);
TwoOneNS::ListNode b2(2, &b3);
TwoOneNS::ListNode b1(1, &b2);
TwoOneNS::ListNode* pNode = TwoOneNS::mergeTwoLists(&a1, &b1);
*/
static ListNode* twoone_mergeTwoLists(ListNode* l1, ListNode* l2)
{
	ListNode tmp(0,nullptr);
	ListNode* pTail = &tmp;
	while (l1&&l2)
	{
		if (l1->value < l2->value)
		{
			pTail->next = l1;
			l1 = l1->next;
		}
		else
		{
			pTail->next = l2;
			l2 = l2->next;
		}
		pTail = pTail->next;
	}

	pTail->next = l1 ? l1:l2;
	return tmp.next;
}



/*
23 merge k sortedlists

Merge K sorted linked lists | Set 1

Given K sorted linked lists of size N each, merge them and print the sorted output.

Example:

Input: k = 3, n =  4
list1 = 1->3->5->7->NULL
list2 = 2->4->6->8->NULL
list3 = 0->9->10->11

Output:
0->1->2->3->4->5->6->7->8->9->10->11
*/
static ListNode* twothree_mergeKLists(std::vector<ListNode*>& lists);


/*
24 swap nodes in pairs
Given a linked list, swap every two adjacent nodes and return its head.

Example:

Given 1->2->3->4, you should return the list as 2->1->4->3.

Note:

	Your algorithm should use only constant extra space.
	You may not modify the values in the list's nodes, only nodes itself may be changed.
*/
ListNode* twofour_swapPairs(ListNode* head);



/*
25 reverse nodes in k-group
Given a linked list, write a function to reverse every k nodes (where k is an input to the function).

Example:
Inputs:  1->2->3->4->5->6->7->8->NULL and k = 3
Output:  3->2->1->6->5->4->8->7->NULL.

Inputs:   1->2->3->4->5->6->7->8->NULL and k = 5
Output:  5->4->3->2->1->8->7->6->NULL.

*/

ListNode* twofive_reverse(ListNode *head, int k);

//26
int twosix_removeDuplicates(int a[], int n);

// 28
const char* twoeight_strStr(const char* str, const char* sub);


/*
33, pivot means the middle is (l+r)/2
 Suppose a sorted array is rotated at some pivot unknown to you beforehand.
(i.e., 0 1 2 4 5 6 7 might become 4 5 6 7 0 1 2).
You are given a target value to search. If found in the array return its index, otherwise return -1.
You may assume no duplicate exists in the array.
*/
int threethree_search(const std::vector<int>& a, int target);


/*
46 backtraking, permutations

Given a collection of numbers, return all possible permutations.

For example,
[1,2,3] have the following permutations:
[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], and [3,2,1].

foursix_permuteDFS(nums, 0, res);
*/

void foursix_permuteDFS(std::vector<int>& nums, int start, std::vector<std::vector<int>>& res);

/*
47 permutations, nums might contain duplicates 
[1,1,2]

[1,1,2][1,2,1][2,1,1]
*/
void foursix_permuteEx(std::vector<int>& nums, int start, std::vector<std::vector<int>>& res);


NS_YY_END