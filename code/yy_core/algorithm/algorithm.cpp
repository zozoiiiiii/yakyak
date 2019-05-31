#include "algorithm.h"


namespace AlgorithmNS
{

	MyBase::MyBase():SIZE(10)
	{

	}

	MyBase::~MyBase()
	{

	}

	void Append(Node* root, int data)
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

	Node* deleteNode(Node* root, int value)
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

	Node* Delete(Node* pNode, int data)
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




	/////hashmap


	long generateHashValue(const char *fname)
	{
		int        i;
		long    hash;
		char    letter;

		hash = 0;
		i = 0;
		while (fname[i] != '\0')
		{
			letter = tolower(fname[i]);
			hash += (long)(letter)*(i + 119);
			i++;
		}

		hash &= (VAR_SIZE - 1);
		return hash;
	}

		HashMap::HashMap()
		{
			m_capacity = VAR_SIZE;
			int n = sizeof(m_table);
			memset(m_table, 0, sizeof(m_table));
			m_nodes = new cvar_t[m_capacity];
			m_size = 0;
			m_node_index = 0;
			m_node_head = NULL;
		}

		HashMap::~HashMap()
		{
			delete[]m_nodes;
		}

		cvar_t * HashMap::get(const char *key)
		{
			assert(strlen(key) < 100);

			long hash;
			cvar_t* kn = find(key);
			if (kn)
				return kn;

			// can not find, then create
			assert(m_node_index < m_capacity);
			cvar_t* kn_new = &m_nodes[m_node_index++];
			strcpy(kn_new->key, key);

			// insert the new node.
			hash = generateHashValue(key);
			kn_new->hash_next = m_table[hash];
			m_table[hash] = kn_new;

			// insert to list
			kn_new->next = m_node_head;
			m_node_head = kn_new;

			return kn_new;
		}

		bool HashMap::isExist(const char* key)
		{
			if (NULL == find(key))
				return false;

			return true;
		}
		cvar_t* HashMap::find(const char* key)
		{
			long hash = generateHashValue(key);
			cvar_t* kn = NULL;
			for (kn = m_table[hash]; kn; kn = kn->hash_next)
			{
				if (strcmp(kn->key, key) == 0)
					return kn;
			}

			return NULL;
		}



		bool PrintDiamond(int a)
		{
			if (a % 2 == 0)
				return false;

			int mid = (a + 1) / 2;

			// half up
			for (int i = 0; i < mid; i++)
			{
				for (int j = 0; j < mid - i - 1; j++)
				{
					printf("-");
				}
				for (int k = 0; k < 2 * i + 1; k++)
				{
					printf("*");
				}
				printf("\n");
			}

			// half down
			for (int i = mid - 2; i >= 0; i--)
			{
				for (int j = 0; j < mid - i - 1; j++)
				{
					printf("-");
				}
				for (int k = 0; k < 2 * i + 1; k++)
				{
					printf("*");
				}
				printf("\n");
			}
			return true;
		}



		BigDataFinder::BigDataFinder()
		{
			m_data = new int[1000];
			memset(m_data, 0, 1000*sizeof(int));
		}

		void BigDataFinder::load(const std::vector<int>& nums)
		{
			int nSize = nums.size();
			for (int i = 0; i < nSize; i++)
			{
				int hashCode;// = generateHashValue(nums[i])
				int index = hashCode % 1000;
				m_data[index] = 1;
			}
		}

		bool BigDataFinder::isNotExist(int nValue)
		{
			int hashCode;// from nValue
			int index = hashCode % 1000;
			return !m_data[index];
		}

		bool BigDataFinder::maybeExist(int nValue)
		{
			int hashCode;// from nValue
			int index = hashCode % 1000;
			return m_data[index];
		}
}