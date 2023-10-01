#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

// size of working set window
const int D = 16;
// set types of pages
const int top = 10;
// set reference bits
const int nob = 4;

// pages are indexed in order: 0,1,2,3,4,5,6,7,8,9,.....

// Check if a row has one
vector<int> CheckWorkingSet(vector<vector<int>> &Bitarr, int top, int nob)
{
    vector<int> RowHasOne(top, 0);
    for (int row = 0; row < top; row++)
    {
        for (int col = 0; col < nob; col++)
        {
            if (Bitarr[row][col] == 1)
            {
                RowHasOne[row] = true;
                break;
            }
        }
    }
    return RowHasOne;
}

void UpdateWorkingSet(vector<vector<int>> &Bitarr, vector<int> &workingSet, int top, int nob)
{
    vector<int> RowContainsOnes = CheckWorkingSet(Bitarr, top, nob);

    // Update working set
    for (int row = 0; row < top; row++)
    {
        // if one of page's reference bits equal 1
        if (RowContainsOnes[row])
        {
            // add top to working set if this page currently doesnot exist in working set
            for (int col = 0; col < nob; col++)
            {
                if (Bitarr[row][col] == 1)
                {
                    if (find(workingSet.begin(), workingSet.end(), row) == workingSet.end())
                        workingSet.push_back(row);
                }
            }
        }
        // all reference bits of page are 0
        else
        {
            // eliminate page from working set if it currently in working set
            if (find(workingSet.begin(), workingSet.end(), row) != workingSet.end())
            {
                vector<int>::iterator it;
                it = find(workingSet.begin(), workingSet.end(), row);
                workingSet.erase(it);
            }
        }
    }
}

// print working set
void printWorkingSet(vector<int> workingSet)
{
    cout << " Working set: ";
    for (int top : workingSet)
    {
        cout << top << " ";
    }
    cout << endl;
}

// print Reference Bit Array (10*4) 
void printBitArray(vector<vector<int>> &Bitarr, int top, int nob)
{
    std::cout << "Reference Bits Array: " << std::endl;
    for (int col = 0; col < top; col++)
    {
        cout<<"Page "<<col<<": ";
        for (int row = 0; row < nob; row++)
        {
            std::cout << Bitarr[col][row] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main()
{
    int listSize = 96;
    vector<int> pageReferenceList(listSize, 0);

    cout <<endl<< "Page list: ";
    // randomly initialize reference page list with 10 types: 0,1,2,3,4,5,6,7,8,9
    for (int i = 0; i < listSize; i++)
    {
        pageReferenceList[i] = rand() % top;
        std::cout << pageReferenceList[i] << " ";
    }
    cout << endl;

    // times of running
    int timesrun = listSize / D;

    vector<int> workingSet;

    // array of reference bits (10*4)
    vector<int> bitvector(nob, 0);
    vector<vector<int>> referenceBits(top, vector<int>(bitvector));

    // run process "timesrun" time
    for (int runth = 0; runth < timesrun; runth++)
    {
        // Reinitialize 1 referenceBits column to 0 before running
        cout << "==================Round " << runth + 1 << "======================" << endl;
        for (int row = 0; row < top; row++)
        {
            referenceBits[row][runth % nob] = 0;
        }

        // Send 16 elements to Window
        int startElement = runth * D;

        cout << "Working set Window: ";
        for (int i = startElement; i < startElement + D; i++)
        {
            int top = pageReferenceList[i];
            cout << top << " ";
            // assign 1 to respective element in referenceBits Array
            referenceBits[top][runth % nob] = 1;
        }
        // Print referenceBits Array
        cout << endl;
        printBitArray(referenceBits, top, nob);

        // Update working set and print working set
        UpdateWorkingSet(referenceBits, workingSet, top, nob);
        printWorkingSet(workingSet);
    }
    return 0;
}
