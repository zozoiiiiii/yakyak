#include "leetcode.h"

NS_YY_BEGIN
ListNode* twothree_mergeKLists(std::vector<ListNode*>& lists)
{
	if (lists.size() == 0)
	{
		return nullptr;
	}

	while (lists.size() > 1)
	{
		std::vector<ListNode*> new_lists;
		for (int i = 0; i + 1 < lists.size(); i += 2)
		{
			ListNode* merged_list = twoone_mergeTwoLists(lists[i], lists[i + 1]);
			new_lists.push_back(merged_list);
		}

		if (lists.size() % 2 == 1)
		{
			new_lists.push_back(lists[lists.size() - 1]);
		}
		lists = new_lists;
	}

	return lists[0];
}


ListNode* twofour_swapPairs(ListNode* head)
{
	if (!head || !head->next)
		return head;

	ListNode* t = head->next;
	head->next = twofour_swapPairs(head->next->next);
	t->next = head;
	return t;


	/*
	ListNode* pNewHead = nullptr;
	ListNode* pNewNode = nullptr;
	ListNode* pNode = head;
	while (pNode && pNode->next)
	{
		ListNode* pCurNode = pNode;
		ListNode* pNextNode = pNode->next;
		pNode = pNode->next;
		pNode = pNode->next;

		
		pCurNode->next = pNextNode->next;

		if (nullptr == pNewHead)
			pNewHead = pNextNode;
		
		if(pNewNode)
			pNewNode->next = pNextNode;

		pNewNode = pNextNode;
		pNewNode->next = pCurNode;

		pNewNode = pNewNode->next;
	}

	return pNewHead;
	*/
}

ListNode* twofive_reverse(ListNode *head, int k)
{
	ListNode* current = head;
	ListNode* next = NULL;
	ListNode* prev = NULL;
	int count = 0;

	/*reverse first k nodes of the linked list */
	while (current != NULL && count < k)
	{
		next = current->next;
		current->next = prev;
		prev = current;
		current = next;
		count++;
	}

	/* next is now a pointer to (k+1)th node
	   Recursively call for the list starting from current.
	   And make rest of the list as next of first node */
	if (next != NULL)
		head->next = twofive_reverse(next, k);

	/* prev is new head of the input list */
	return prev;
}


int twosix_removeDuplicates(int a[], int n)
{
	if (n < 2)
		return n;

	int id = 1;
	for (int i = 1; i < n; ++i)
	{
		if (a[i] != a[i - 1])
			a[id++] = a[i];
	}

	return id;
}


const char* twoeight_strStr(const char* str, const char* sub)
{
	if (nullptr == str || nullptr == sub)
		return nullptr;

	for (; *str; str++)
	{
		const char* h = str;
		const char* n = sub;
		for (; *h&&*n && (*h == *n); ++h, ++n) {}

		if (*n == '\0')
			return str;
	}

	return nullptr;

}


int threethree_search(const std::vector<int>& a, int target)
{
	int l = 0;
	int r = a.size() - 1;
	while (l <= r)
	{
		int m = (l + r) / 2;
		if (a[m] == target)
			return m;
		if (a[m] >= a[l])
		{
			if (a[l] <= target && target <= a[m])
				r = m - 1;
			else
				l = m + 1;
		}
		else
		{
			if (a[m] >= target && target >= a[l])
				r = m - 1;
			else
				l = m + 1;
		}

		return -1;
	}
}


void foursix_permuteDFS(std::vector<int>& nums, int start, std::vector<std::vector<int>>& res)
{
	if (start >= nums.size())
		res.push_back(nums);

	for (int i = start; i < nums.size(); i++)
	{
		swap(nums[start], nums[i]);
		foursix_permuteDFS(nums, start + 1, res);
		swap(nums[i], nums[start]);
	}
}


void foursix_permuteEx(std::vector<int>& nums, int start, std::vector<std::vector<int>>& res)
{
	if (start >= nums.size())
		res.push_back(nums);

	for (int i = start; i < nums.size(); i++)
	{
		if (i != start && nums[i] == nums[start])
			continue;

		swap(nums[start], nums[i]);
		foursix_permuteDFS(nums, start + 1, res);
		swap(nums[i], nums[start]);
	}
}

NS_YY_END