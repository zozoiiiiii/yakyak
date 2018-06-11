/************************************************************************/
/*
@author:  junliang
@brief:   搜索相关

二分搜索，深度优先搜索，宽度优先搜索
@time:    20130516
*/
/************************************************************************/

//bstree.h, 二叉排序树

//二分查找(折半查找), 数组要有序
int BinarySearch(int* arr, int start, int end, int key)
{
    int left,right;
    int mid;
    left=start;
    right=end;

    while (left<=right)
    {
        mid=(left+right)/2;

        if (key<arr[mid])
        {
            right=mid-1;
        }
        else if(key>arr[mid])
        {
            left=mid+1;
        }
        else
            return mid;
    }

    return -1;
}