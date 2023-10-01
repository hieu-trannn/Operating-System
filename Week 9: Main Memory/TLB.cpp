#include <iostream>
#include <bitset>
#include <vector>

using namespace std;

// Struct of TLB or pageNumber table
struct LookupTable
{
    int pageNumber;
    int frameNumber;
};

int main()
{
    vector<LookupTable> tlb, pageTable;

    pageTable.resize(8);

    // Initialize the pageNumber table randomly
    for (int i = 0; i < 8; i++)
    {
        pageTable[i].pageNumber = i;
        pageTable[i].frameNumber = rand() % 8;
    }
    // Print the binary pageNumber table
    cout << "Page Table:" << endl;
    for (auto &entry : pageTable)
    {
        cout << "Page: " << bitset<3>(entry.pageNumber) << " Frame: " << bitset<3>(entry.frameNumber) << endl;
    }

    while (1)
    {
        string input;
        cout << "Enter logical address (8 binary bits): ";
        cin >> input;

        if (input.size() != 8)
        {
            cout << "Invalid input. Try again" << endl;
            continue;
        }

        // Convert binary string to integer
        int logicalAddress = stoi(input, 0, 2);

        // Get pageNumber number and offset
        int pageNumber = (logicalAddress & 0b11100000) >> 5;
        int offset = logicalAddress & 0b00011111;

        int frameNumber;

        // Check if TLB has pageNumber-frameNumber case
        bool TlbHave = false;

        for (auto &entry : tlb)
        {
            if (entry.pageNumber == pageNumber)
            {
                frameNumber = entry.frameNumber;
                TlbHave = true;
                break;
            }
        }

        // If TLB has pageNumber-frameNumber case, print the physical address
        if (TlbHave)
        {
            int physicalAddress = (frameNumber << 5) + offset;
            cout << "Physical Address (TLB search): " << bitset<8>(physicalAddress) << endl;
        }
        else
        {
            // Search pageNumber table
            for (auto &entry : pageTable)
            {
                if (entry.pageNumber == pageNumber)
                {
                    frameNumber = entry.frameNumber;
                    break;
                }
                // cout << "pageNumber number not found in pageNumber table." << endl;
            }
            // Add the pageNumber-FrameNumber case to TLB if TLB's size smaller than 4
            if (tlb.size() < 4)
            {
                tlb.push_back({pageNumber, frameNumber});
            }
            else
            {
                tlb.erase(tlb.begin());
                tlb.push_back({pageNumber, frameNumber});
            }

            int physicalAddress = (frameNumber << 5) + offset;
            cout << "Physical Address (page table search): " << bitset<8>(physicalAddress) << endl;

            // print TLB (TLB changes)
            cout << "TLB Changes!!" << endl;
            for (auto &entry : tlb)
            {
                cout << "pageNumber: " << bitset<3>(entry.pageNumber) << " Frame: " << bitset<3>(entry.frameNumber) << endl;
            }
        }
        cout<< "<===========================================>" <<endl;
    }

    return 0;
}