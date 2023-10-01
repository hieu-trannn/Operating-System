#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

// const int MAX_RESOURCES = 10; // m
// const int MAX_PROCESSES = 5;  // n

// vector<int> providedResources(MAX_RESOURCES);
// vector<int> available(MAX_RESOURCES);
// vector<vector<int>> maximum(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
// vector<vector<int>> allocation(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
// vector<vector<int>> need(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
// vector<bool> finished(MAX_PROCESSES, false);

// int providedResources[MAX_RESOURCES];
// int available[MAX_RESOURCES];
// int maximum[MAX_PROCESSES][MAX_RESOURCES];
// int allocation[MAX_PROCESSES][MAX_RESOURCES];
// int need[MAX_PROCESSES][MAX_RESOURCES];
// int finished[MAX_PROCESSES] = {0};

// available + allocation
bool isSafeState(int nor, int nop, int available[], int allocation[][5], int need[][5], int finished[])
{
    int* work = available;
    // vector<bool> canFinish(MAX_PROCESSES, false);
    int finishedProcesses = 0;

    while (finishedProcesses < nop)
    {
        bool found = false;

        for (int i = 0; i < nop; ++i)
        {
            if (!finished[i])
            {
                bool canExecute = true;

                for (int j = 0; j < nor; ++j)
                {
                    if (need[i][j] > work[j])
                    {
                        canExecute = false;
                        break;
                    }
                }

                if (canExecute)
                {
                    for (int j = 0; j < nor; ++j)
                    {
                        work[j] += allocation[i][j];
                    }

                    // canFinish[i] = true;
                    finished[i] = true;
                    finishedProcesses++;
                    found = true;
                }
            }
        }

        if (!found)
        {
            // Deadlock detected
            return false;
        }
    }

    return true;
}

bool requestResources(int processId, vector<int> &request, vector<bool> finished, vector<int> available, vector<vector<int>> allocation, vector<vector<int>> need)
{
    for (int i = 0; i < MAX_RESOURCES; ++i)
    {
        // loi here
        if (request[i] > need[processId][i] || request[i] > available[i])
        {
            // Requested resources exceed the maximum need or available resources

            cout << "bigger than max" << endl;
            return false;
        }
    }

    for (int i = 0; i < MAX_RESOURCES; ++i)
    {
        available[i] -= request[i];
        allocation[processId][i] += request[i];
        need[processId][i] -= request[i];
    }
    cout << "Resources after allocate: " << endl;

    if (!isSafeState(available, allocation, need, finished))
    {
        // Requested resources would result in an unsafe state
        // Roll back changes and return false
        cout << "isn't safe" << endl;
        for (int i = 0; i < MAX_RESOURCES; ++i)
        {
            available[i] += request[i];
            allocation[processId][i] -= request[i];
            need[processId][i] += request[i];
        }

        return false;
    }

    return true;
}

// void releaseResources(int processId, vector<int>& release)
// {
//     for (int i = 0; i < MAX_RESOURCES; ++i)
//     {
//         available[i] += release[i];
//         allocation[processId][i] -= release[i];
//         need[processId][i] += release[i];
//     }
// }

int main()
{
    // Read the total number of resources
    // int numResources;
    // cout << "Enter the total number of resources: ";
    // cin >> numResources;
    int numResources = 3;

    // Read the total number of processes
    // int numProcesses;
    // cout << "Enter the total number of processes: ";
    // cin >> numProcesses;
    int numProcesses = 5;
    vector<int> providedResources;
    vector<int> available;
    vector<vector<int>> maximum(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
    vector<vector<int>> allocation(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
    vector<vector<int>> need(MAX_PROCESSES, vector<int>(MAX_RESOURCES));
    // vector<vector<int>> maximum;
    // vector<vector<int>> allocation;
    // vector<vector<int>> need;
    vector<bool> finished(MAX_PROCESSES, false);

    // vector<int> providedResources;
    // // Read the available resources
    // cout << "Enter the number of available resources for each type: ";
    // for (int i = 0; i < numResources; ++i)
    // {
    //     cin >> available[i];
    // }

    // Read the provided resources
    // cout << "Enter the number of provided resources for each type: ";
    // for (int i = 0; i < numResources; ++i)
    // {
    //     cin >> providedResources[i];
    // }
    providedResources[0] = 10;
    providedResources[1] = 5;
    providedResources[2] = 7;

    // Read the maximum resource needs for each process
    // cout << "Enter the maximum resource needs for each process: " << endl;
    // for (int i = 0; i < numProcesses; ++i)
    // {
    //     cout << "For process " << i << endl;
    //     for (int j = 0; j < numResources; ++j)
    //     {
    //         cin >> maximum[i][j];
    //     }
    // }
    maximum[0][0] = 7;
    maximum[0][1] = 5;
    maximum[0][2] = 3;
    maximum[1][0] = 3;
    maximum[1][1] = 2;
    maximum[1][2] = 2;
    maximum[2][0] = 9;
    maximum[2][1] = 0;
    maximum[2][2] = 2;
    maximum[3][0] = 2;
    maximum[3][1] = 2;
    maximum[3][2] = 2;
    maximum[4][0] = 4;
    maximum[4][1] = 3;
    maximum[4][2] = 3;

    // Read the current resource allocations for each process
    // cout << "Enter the current resource allocations for each process: ";

    allocation[0][0] = 0;
    allocation[0][1] = 1;
    allocation[0][2] = 0;
    allocation[1][0] = 2;
    allocation[1][1] = 0;
    allocation[1][2] = 0;
    allocation[2][0] = 3;
    allocation[2][1] = 0;
    allocation[2][2] = 2;
    allocation[3][0] = 2;
    allocation[3][1] = 1;
    allocation[3][2] = 1;
    allocation[4][0] = 0;
    allocation[4][1] = 0;
    allocation[4][2] = 2;

    for (int i = 0; i < numProcesses; ++i)
    {
        for (int j = 0; j < numResources; ++j)
        {
            // cin >> allocation[i][j];
            available[j] = providedResources[j] - allocation[i][j];
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
    for (int i = 0; i < numResources; ++i)
    {
        cout << available[i] << endl;
    }

    // Perform resource requests and releases
    char choice;
    do
    {
        int processId;
        cout << "Enter the process ID (0-" << numProcesses - 1 << "): ";
        cin >> processId;

        vector<int> request(numResources);
        cout << "Enter the resource request for process " << processId << ": ";
        for (int i = 0; i < numResources; ++i)
        {
            cin >> request[i];
        }

        if (requestResources(processId, request, finished, available, allocation, need))
        {
            cout << "Request granted. Resources allocated." << endl;
        }
        else
        {
            cout << "Request denied. Resources not allocated." << endl;
        }

        cout << "Do you want to make another request/release? (y/n): ";
        cin >> choice;
    } while (choice == 'y' || choice == 'Y');

    return 0;
}
