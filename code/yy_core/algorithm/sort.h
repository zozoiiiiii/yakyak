/************************************************************************/
/*
@author:  junliang
@brief:   �������

    �Ƚ�����:ͨ����������Ԫ�ؽ��бȽ�������.
            �������򣬶�����ѡ�����򣬹鲢����Ϳ�������ð������
    �ǱȽ�����: �������򣬻�������Ͱ����

sample:
    int num[5]={5,8,1,4,7};
    BubbleSort(num, 5);



@time:    20130516
*/
/************************************************************************/




//ð������, ����һ�ˣ������Ƚϣ�����˳�򻥻�, O(n^2)
void BubbleSort(int arr[], int n)
{
    for(int i=0; i<n; i++)
    for(int j=i+1; j<n; j++)
    {
        if(arr[i]>arr[j])
        {
            //����
            arr[i]^=arr[j];
            arr[j]^=arr[i];
            arr[i]^=arr[j];
        }
    }
}


//��������, ð���㷨�ĸĽ���ͨ��һ�������Ҫ��������ݷָ�ɶ����������֣�����һ���ֱ���һ���ֶ�С���ֶ���֮���ݹ顣O(nlogn)
void QuickSort(int* arr, int left, int right)
{
    //��Ҫ�Ƚϵ����ݴ�, �±�Ϊleft�Ŀ��������޸���
    int key=arr[left];
    int i=left;
    int j=right;
    while(i<j)
    {
        //������β�ҵ���һ����keyС��
        while(arr[j]>key&&i<j)j--;
        //��ֵ�����
        arr[i]=arr[j];

        //������ͷ�ҵ���һ����key���
        while(arr[i]<key&&i<j)i++;
        //��ֵ���ұ�
        arr[j]=arr[i];
    }

    arr[i]=key;

    if(i-1>left)
        QuickSort(arr, left, i-1);

    if(j+1<right)
        QuickSort(arr, j+1, right);
}

//��100000000�����������ҳ�����10000����Ҫ��ʱ�临�Ӷ��š�
void QuickSortT(int* arr, int left, int right)
{
    //��Ҫ�Ƚϵ����ݴ�, �±�Ϊleft�Ŀ��������޸���
    int key=arr[left];
    int i=left;
    int j=right;
    while(i<j)
    {
        //������β�ҵ���һ����key���
        while(arr[j]<key&&i<j)j--;
        //��ֵ�����
        arr[i]=arr[j];

        //������ͷ�ҵ���һ����keyС��
        while(arr[i]<key&&i<j)i++;
        //��ֵ���ұ�
        arr[j]=arr[i];
    }

    arr[i]=key;

    if(i-1>left && i>10000 && left<10000)
        QuickSort(arr, left, i-1);

    if(i-1<right && i<10000 && right>10000)
        QuickSort(arr, i+1, right);
}

//ѡ������ð������ĸĽ�������һ�ˣ��ҳ�һ����Сֵ����Сֵ�Ͷ�Ӧλ�û���, O(n^2)
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
            //����
            arr[i]^=arr[min];
            arr[min]^=arr[i];
            arr[i]^=arr[min];
        }
    }
}

//������ ѡ������ĸĽ�������һ���ҳ���Сֵ�Ĺ����У������ȽϽ�������ٱȽϴ���, O(nlogn)