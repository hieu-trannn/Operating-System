#include <iostream>
#include <bitset>
#include <unordered_map>
#include <vector>

using namespace std;

// Struct to represent a TLB entry, or page table
struct TLBEntry
{
    int pageNumber;
    int frameNumber;
};

int main()
{
    vector<TLBEntry> tlb;
    tlb.resize(8); // Set the size of TLB to 4

    // Initialize the page table with random frame numbers
    // unordered_map<int, int> pageTable;
    for (int i = 0; i < 8; ++i)
    {
        // pageTable[i] = rand() % 8; // Random frame number from 0 to 7
        tlb[i].pageNumber = i;
        tlb[i].frameNumber = rand() % 8;
    }
    // Print the initial page table
    cout << "Initial Page Table:" << endl;
    for (const auto &entry : tlb)
    {
        cout << "Page: " << bitset<3>(entry.pageNumber) << " - Frame: " << bitset<3>(entry.frameNumber) << endl;
    }
    cout << endl;

    vector<TLBEntry>::iterator ptr;
    for (ptr = tlb.begin(); ptr < tlb.end(); ptr++)
    // cout << *ptr << " "
    {
        if (ptr->pageNumber == 9)
        {
            cout<<"Find frame: "<<endl;
            break;
        }
    }
    return 0;
}