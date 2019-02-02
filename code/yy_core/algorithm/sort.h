/************************************************************************/
/*
@author:  junliang
@brief:   sort

	sort by compare:插入排序，堆排序，选择排序，归并排序和快速排序，冒泡排序
	sort not by compare: 计数排序，基数排序，桶排序

sample:
    int num[5]={5,8,1,4,7};
    BubbleSort(num, 5);



@time:    20130516
*/
/************************************************************************/




//O(n*n)
void BubbleSort(int arr[], int n)
{
    for(int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
    {
        if(arr[i]>arr[j])
        {
			// swap
            arr[i]^=arr[j];
            arr[j]^=arr[i];
            arr[i]^=arr[j];
        }
    }
}

//better than BubbleSort, O(n^2)
void SelectSort(int* arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[min] > arr[j])
				min = j;
		}

		if (min != i)
		{
			//swap
			arr[i] ^= arr[min];
			arr[min] ^= arr[i];
			arr[i] ^= arr[min];
		}
	}
}


//use recursive, O(nlogn)
void QuickSort(int* arr, int left, int right)
{
    int key=arr[left];
    int i=left;
    int j=right;
    while(i<j)
    {
		// find from the end, the first value which smaller than key
        while(arr[j]>key&&i<j)j--;
        arr[i]=arr[j];

		// find from front, the first value which bigger than key.
        while(arr[i]<key&&i<j)i++;
        arr[j]=arr[i];
    }

    arr[i]=key;

    if(i-1>left)
        QuickSort(arr, left, i-1);

    if(j+1<right)
        QuickSort(arr, j+1, right);
}

// find the biggest 1000 floats from 1million floats
// all the floats is not possible to load ram, so we need to seperate into some files, then read these files
// keep the top 10000 values.   
void QuickSortT(int* arr, int left, int right)
{
}

// 二叉树排序, bstree.h
//堆排序,heapsort, O(nlogn)