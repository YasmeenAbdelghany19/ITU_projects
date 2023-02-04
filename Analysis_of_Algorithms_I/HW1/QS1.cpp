#include <iostream>
#include <chrono>

using namespace std;

// global variables
int swap_count = 0;
int partitions_count = 0;

int partition(int *Arr, int Low, int High)
{
    int i = Low, j = Low;
    int Pivot = Arr[High];
    while (j != High)
    {
        if (Arr[i] <= Pivot && Arr[j] <= Pivot)
        {
            i++;
            j++;
        }
        else if (Arr[j] > Pivot)
        {
            j++;
            if (j == High)
            {
                swap(Arr[i], Arr[High]);
                swap_count++;
                break;
            }
            swap(Arr[i], Arr[j]);
            swap_count++;
            // increment i, the pivot should be placed after Arr[i] bec it's smaller
            if (Arr[i] <= Pivot)
            {
                i++;
            }
        }
    }

    return i;
}
void quickSort(int *Arr, int Low, int High)
{
    if (Low < High)
    {
        int i = partition(Arr, Low, High);
        partitions_count++;
        quickSort(Arr, Low, i - 1);
        quickSort(Arr, i + 1, High);
    }
}

int main()
{
    int size = 0;
    cout << "Enter array size: ";
    cin >> size;
    cout << "Enter array elements: ";
    int arr[size] = {};
    for (int i = 0; i < size; i++)
    {
        cin >> arr[i];
    }

    int Low = 0;
    int High = size - 1;
    auto begin = chrono::high_resolution_clock::now(); // start clock
    quickSort(arr, Low, High);
    auto end = chrono::high_resolution_clock::now(); // end clock
    double time = chrono::duration_cast<chrono::duration<double>>(end - begin).count();
    cout << "Sorted Array: ";
    for (int i = 0; i < size; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
    cout << "Number of swaps: " << swap_count << endl;
    cout << "Number of partitions: " << partitions_count << endl;
    cout << "Time Elapsed in seconds: " << time << endl
         << endl;
    return 0;
}