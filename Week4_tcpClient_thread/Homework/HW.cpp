/*
Viết chương trình C với yêu cầu sau:
- Thread mẹ khởi tạo ready = 0, rồi tạo thread con.
- Thread mẹ đọc giá trị x, y từ bàn phím xong và gán ready = 1, 
sau đó chờ đến khi ready == 0 thì in giá trị z và kết thúc.
- Thread con chờ đến khi ready == 1 thì tính z = x + y, rồi gán ready = 0 và kết thúc.
*/

/*
Steps: 
    1: cd directory containing code file
    2: Compile:
        g++ -pthread HW.cpp -o HW
    3. RUN executable file:
        ./HW
*/
#include <iostream>
#include <thread>

using namespace std;

// alocate shared memory (struct)
struct shared_data
{
    int x;
    int y;
    int z;
    int ready;
}data;
// shared_data data;

int main()
{
    /*Parent thread*/
    
    // initialize ready = 0
    data.ready = 0;
    cout<<"Hello from parent thread\n";
    cout<<"Input for x,y: "<<endl;
    cin>>data.x>>data.y;

    // create child thread
    thread t([](){
        cout<< "Hello from child thread\n";
        // wait ready = 1
        if (data.ready == 1)
        {
            data.z = data.x + data.y;
            data.ready = 0;
        }
    });
    /* parent thread */
    data.ready = 1;
    //wait child thread
    t.join();
    cout<<"Child thread finished\n";
    cout<<"Value of z: "<<data.z<<endl;
    return 0;
}