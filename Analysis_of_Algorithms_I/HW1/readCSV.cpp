#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

//global variables
int swap_count = 0;
int partitions_count = 0;

int partition(vector<vector<float>> &Arr, int Low, int High)
{
    int i = Low, j = Low;
    float Pivot = Arr[High][1];
    while (j != High)
    {
        if (Arr[i][1] <= Pivot && Arr[j][1] <= Pivot)
        {
            i++;
            j++;
        }
        else if (Arr[j][1] > Pivot)
        {
            j++;
            if (j == High)
            {
                swap(Arr[i], Arr[High]);    //swap pivot with the partition index i when j reaches the end of subarray
                swap_count++;
                break;
            }
            swap(Arr[i], Arr[j]);
            swap_count++;
            if (Arr[i][1] <= Pivot)
            {
                i++;            // increment i, the pivot could be placed after Arr[i] bec it's smaller
            }
        }
    }
    return i;
}
void quickSort(vector<vector<float>> &Arr, int Low, int High)
{
    if (Low < High)
    {
        int i = partition(Arr, Low, High);
        partitions_count++;
        quickSort(Arr, Low, i - 1);
        quickSort(Arr, i + 1, High);
    }
}

void sort_data(vector<vector<float>> &books_ratings, int len)
{
    int Low = 0;
    int High = len - 1;
    swap_count = 0;
    partitions_count = 0;

    auto begin = chrono::high_resolution_clock::now(); //start clock
    quickSort(books_ratings, Low, High);
    auto end = chrono::high_resolution_clock::now(); //end clock
    double time = chrono::duration_cast<chrono::duration<double>>(end - begin).count();

    cout << "Number of rows: " << len << endl;
    cout << "Number of swaps: " << swap_count << endl;
    cout << "Number of partitions: " << partitions_count << endl;
    cout << "Time Elapsed in seconds: " << time << endl
         << endl;
}

int main()
{
    string throw_msg = "Could not open/find the file\n";
    vector<vector<float>> books_ratings;    //stores all rows and both columns except header
    vector<float> row;   
    vector<string> header_row;
    string header_cells[1][2];              //stores header labels
    bool header = true;
    string line = "";
    string cell = "";
    int i = 0;

    // reading from books.csv
    fstream myFile;
    try
    {
        myFile.open("books.csv", ios::in); // ios::in for reading file
        if (myFile.fail())
        {
            throw throw_msg;
        }
        while (getline(myFile, line))
        {
            stringstream str(line);
            int j = 0;
            row.clear();
            while (getline(str, cell, ','))
            {
                if (j == 0 || j == 3)   // read only bookID and average_ratings
                {
                    if (i < 2)
                    {
                        header_row.push_back(cell);
                        i++;
                    }
                    else
                    {
                        row.push_back(stof(cell));  //cast string to float before pushing into row vector
                    }
                }
                j++;
            }
            if (!header)
            {
                books_ratings.push_back(row);
            }
            else
            {
                header = false;
                header_cells[0][0] = header_row[0];
                header_cells[0][1] = header_row[1];
            }
        }
        myFile.close();
    }
    catch (string error)
    {
        cout << error;
        exit(1);
    }

    // sorting average_ratings using full, half, and quarter of the data
    int len = books_ratings.size();
    sort_data(books_ratings, len);              // full data statistics
    //sort_data(books_ratings, len / 2);        // uncomment to check half of the data statistics
    //sort_data(books_ratings, len / 4);        // uncomment to check quarter of the data statistics

    // writing the sorted average ratings column and the corresponding bookID into a file
    fstream myFile1("sorted_books.csv", ios::out); // ios::out for writing into the file
    myFile1 << header_cells[0][0] << "," << header_cells[0][1] << endl;
    for (int i = 0; i < len; i++)
    {
        myFile1 << books_ratings[i][0] << "," << books_ratings[i][1] << endl;
    }
    myFile1.close();

    return 0;
}
