/************************************************************************/
/*
@author:  junliang
@brief:   排序相关

    比较排序:通过对数组中元素进行比较来排序.
            插入排序，堆排序，选择排序，归并排序和快速排序，冒泡排序
    非比较排序: 计数排序，基数排序，桶排序

sample:
    int num[5]={5,8,1,4,7};
    BubbleSort(num, 5);



@time:    20130516
*/
/************************************************************************/




//冒泡排序, 遍历一趟，两两比较，根据顺序互换, O(n^2)
void BubbleSort(int arr[], int n)
{
    for(int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
    {
        if(arr[i]>arr[j])
        {
            //互换
            arr[i]^=arr[j];
            arr[j]^=arr[i];
            arr[i]^=arr[j];
        }
    }
}


//快速排序, 冒泡算法的改进，通过一趟排序把要排序的数据分割成独立的两部分，其中一部分比另一部分都小，分而治之，递归。O(nlogn)
void QuickSort(int* arr, int left, int right)
{
    //把要比较的数暂存, 下标为left的可以任意修改了
    int key=arr[left];
    int i=left;
    int j=right;
    while(i<j)
    {
        //从数组尾找到第一个比key小的
        while(arr[j]>key&&i<j)j--;
        //赋值给左边
        arr[i]=arr[j];

        //从数组头找到第一个比key大的
        while(arr[i]<key&&i<j)i++;
        //赋值给右边
        arr[j]=arr[i];
    }

    arr[i]=key;

    if(i-1>left)
        QuickSort(arr, left, i-1);

    if(j+1<right)
        QuickSort(arr, j+1, right);
}

//在100000000个浮点数中找出最大的10000个，要求时间复杂度优。
void QuickSortT(int* arr, int left, int right)
{
    //把要比较的数暂存, 下标为left的可以任意修改了
    int key=arr[left];
    int i=left;
    int j=right;
    while(i<j)
    {
        //从数组尾找到第一个比key大的
        while(arr[j]<key&&i<j)j--;
        //赋值给左边
        arr[i]=arr[j];

        //从数组头找到第一个比key小的
        while(arr[i]<key&&i<j)i++;
        //赋值给右边
        arr[j]=arr[i];
    }

    arr[i]=key;

    if(i-1>left && i>10000 && left<10000)
        QuickSort(arr, left, i-1);

    if(i-1<right && i<10000 && right>10000)
        QuickSort(arr, i+1, right);
}

//选择排序，冒泡排序的改进，遍历一趟，找出一个最小值，最小值和对应位置互换, O(n^2)
void SelectSort(int* arr, int n)
{
    for(int i=0; i<n; i++)
    {
        int min=i;
        for(int j=i+1; j<n; j++)
        {
            if(arr[min]>arr[j])
                min=j;
        }

        if(min!=i)
        {
            //互换
            arr[i]^=arr[min];
            arr[min]^=arr[i];
            arr[i]^=arr[min];
        }
    }
}

//堆排序， 选择排序的改进，遍历一趟找出最小值的过程中，保留比较结果，减少比较次数, O(nlogn)