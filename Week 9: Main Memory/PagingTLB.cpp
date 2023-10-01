#include <iostream>
#include <bitset>
#include <unordered_map>
#include <vector>

using namespace std;

// Struct to represent a TLB entry
struct TLBEntry
{
    int pageNumber;
    int frameNumber;
};

// Function to print the TLB state
void printTLB(const vector<TLBEntry> &tlb)
{
    cout << "TLB State:" << endl;
    for (const auto &entry : tlb)
    {
        cout << "Page: " << bitset<3>(entry.pageNumber) << " - Frame: " << bitset<3>(entry.frameNumber) << endl;
    }
    cout << endl;
}

int main()
{
    // Initialize the TLB
    vector<TLBEntry> tlb(4);
    tlb.reserve(4);
    // tlb.resize(4); // Set the size of TLB to 4

    // Initialize the page table with random frame numbers
    unordered_map<int, int> pageTable;
    for (int i = 0; i < 8; ++i)
    {
        pageTable[i] = rand() % 8; // Random frame number from 0 to 7
    }

    // Print the initial page table
    cout << "Initial Page Table:" << endl;
    for (const auto &entry : pageTable)
    {
        cout << "Page: " << bitset<3>(entry.first) << " - Frame: " << bitset<3>(entry.second) << endl;
    }
    cout << endl;

    // Start address translation process
    while (true)
    {
        string input;
        cout << "Enter a logical address (8 bits binary): ";
        cin >> input;

        if (input.size() != 8)
        {
            cout << "Invalid address length. Please try again." << endl;
            continue;
        }

        // Convert binary input to integer
        int logicalAddress = stoi(input, 0, 2);

        // Extract page number and offset
        int pageNumber = (logicalAddress & 0b11100000) >> 5;
        int offset = logicalAddress & 0b00011111;
        // cout << "Off set: " << offset << endl;
        // Check if TLB has the translation
        bool tlbHit = false;
        int frameNumber;

        for (const auto &entry : tlb)
        {
            if (entry.pageNumber == pageNumber)
            {
                frameNumber = entry.frameNumber;
                // cout<<"Frame Number: "<<frameNumber<<endl;
                tlbHit = true;
                break;
            }
        }

        // If TLB hit, print the result and TLB state
        if (tlbHit)
        {
            int physicalAddress = (frameNumber << 5) | offset;
            cout << "Physical Address (TLB hit): " << bitset<8>(physicalAddress) << endl;
            // printTLB(tlb); Tlb khong thay doi?
        }
        else
        {
            // TLB miss, perform page table lookup
            auto it = pageTable.find(pageNumber);
            if (it != pageTable.end())
            {
                frameNumber = it->second;

                // Add the translation to TLB (FIFO replacement)
                if (tlb.size() < 4)
                {
                    tlb.push_back({pageNumber, frameNumber});
                }
                else
                {
                    tlb.erase(tlb.begin());
                    tlb.push_back({pageNumber, frameNumber});
                }

                int physicalAddress = (frameNumber << 5) | offset;
                cout << "Physical Address (TLB miss): " << bitset<8>(physicalAddress) << endl;
                printTLB(tlb);
            }
            else
            {
                cout << "Page Fault! Page number not found in page table." << endl;
            }
        }
    }

    return 0;
}
