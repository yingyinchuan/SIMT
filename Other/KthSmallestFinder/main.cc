#include <iostream>
#include <vector>
#include <algorithm>

int partition(std::vector<int> &arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (arr[j] <= pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

int kthSmallest(std::vector<int> &arr, int low, int high, int k)
{
    if (k > 0 && k <= high - low + 1)
    {
        int partitionIndex = partition(arr, low, high);

        if (partitionIndex - low == k - 1)
        {
            return arr[partitionIndex];
        }
        else if (partitionIndex - low > k - 1)
        {
            return kthSmallest(arr, low, partitionIndex - 1, k);
        }
        else
        {
            return kthSmallest(arr, partitionIndex + 1, high, k - partitionIndex + low - 1);
        }
    }

    // 如果k不在有效范围内，返回一个足够大的值表示错误
    return 2147483647;
}

int main()
{
    std::vector<int> arr = {12, 3, 5, 7, 4, 19, 26};
    int k = 3;

    int result = kthSmallest(arr, 0, arr.size() - 1, k);

    std::cout << "The " << k << "th smallest element is: " << result << std::endl;

    return 0;
}
