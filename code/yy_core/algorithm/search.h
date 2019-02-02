/************************************************************************/
/*
@author:  junliang
@brief:   
@time:    20130516
*/
/************************************************************************/


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