#include <iostream>
#include <vector>

using namespace std;

// return 1 if process isn't finished and its need is satisfied by current work
bool Executable(vector<bool> &Finish, int process, int numResource, vector<vector<int>> need, vector<int> work)
{
    if (!Finish[process])
    {
        for (int j = 0; j < numResource; ++j)
        {
            if (need[process][j] > work[j])
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }
    return true;
}

//return 1 if in the safe state (all process can be executed)
bool SafeState(vector<int> &available, vector<vector<int>> &max, vector<vector<int>> &allocation, vector<vector<int>> &need, int numProcess, int numResource)
{
    vector<int> work = available;
    vector<bool> Finish(numProcess, false);
    vector<int> SafeList;

    int finishedProcess = 0;
    while (finishedProcess < numProcess)
    {
        bool foundExecutableProcess = false;
        for (int i = 0; i < numProcess; ++i)
        {
            if (Executable(Finish, i, numResource, need, work))
            {
                for (int j = 0; j < numResource; ++j)
                {
                    work[j] = work[j] + allocation[i][j];
                }
                Finish[i] = true;
                foundExecutableProcess = true;
                finishedProcess++;
                SafeList.push_back(i);
            }
        }

        if (!foundExecutableProcess)
        {
            for (int i = 0; i < numProcess; i++)
            {
                if (Finish[i] == 0)
                {
                    return false;
                }
            }
        }
    }
    //print safe list
    cout << "Safe List: ";
    for (int i = 0; i < numProcess - 1; ++i)
    {
        cout << SafeList[i] << " -> ";
    }
    cout << SafeList[numProcess - 1] << endl;
    
    return true;
}

//return 1 if request is valid (still in safe state)
bool isRequestValid(vector<int> &available, vector<vector<int>> &max, vector<vector<int>> &allocation, vector<vector<int>> &need, int processId, vector<int> &request, int numProcess, int numResource)
{
    for (int i = 0; i < numResource; ++i)
    {
        if (request[i] > need[processId][i])
        {
            return false;
        }
        if (request[i] > available[i])
        {
            return false;
        }
    }

    //pretend allocate resouces to request
    for (int i = 0; i < numResource; ++i)
    {
        available[i] -= request[i];
        allocation[processId][i] += request[i];
        need[processId][i] -= request[i];
    }

    if (!SafeState(available, max, allocation, need, numProcess, numResource))
    {
        cout << "Can't reach Safe State" << endl;
        //get back resources
        for (int i = 0; i < numResource; ++i)
        {
            available[i] += request[i];
            allocation[processId][i] -= request[i];
            need[processId][i] += request[i];
        }
        return false;
    }
    return true;
}

int main()
{
    int numProcess, numResource;
    cout << "Number of processes: ";
    cin >> numProcess;
    cout << "Number of resources: ";
    cin >> numResource;
    vector<int> providedResources(numResource);
    vector<int> available(numResource);
    vector<vector<int>> max(numProcess, vector<int>(numResource));
    vector<vector<int>> allocation(numProcess, vector<int>(numResource));
    vector<vector<int>> need(numProcess, vector<int>(numResource));

    cout << "Provided resources: \n";
    for (int i = 0; i < numResource; ++i)
    {
        cin >> providedResources[i];
        available[i] = providedResources[i];
    }

    cout << "Maximum matrix:\n";
    for (int i = 0; i < numProcess; ++i)
    {
        for (int j = 0; j < numResource; ++j)
        {
            cin >> max[i][j];
        }
    }
    cout << "Allocation matrix: \n";
    for (int i = 0; i < numProcess; ++i)
    {
        for (int j = 0; j < numResource; ++j)
        {
            cin >> allocation[i][j];
        }
    }
    
    //need matrix
    for (int i = 0; i < numProcess; i++)
    {
        for (int j = 0; j < numResource; j++)
        {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    //count available
    for (int i = 0; i < numProcess; i++)
    {
        for (int j = 0; j < numResource; j++)
        {
            available[j] -= allocation[i][j];
        }
    }

    cout << "Process ID for resource request: ";
    int processId;
    cin >> processId;

    vector<int> request(numResource);
    cout << "Resource request for process " << processId << ":\n";
    for (int i = 0; i < numResource; ++i)
    {
        cin >> request[i];
    }

    if (isRequestValid(available, max, allocation, need, processId, request, numProcess, numResource))
    {
        cout << "Request executed" << endl;
    }
    else
    {
        cout << "Request invalid. Try again" << endl;
    }
    return 0;
}
