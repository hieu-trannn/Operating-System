/*
- Nhóm 1-3: SJF (SJN); Nhóm 4-6: SRT; Nhóm 7-9: EDF
- Input: Thông tin số process, arrival time, burst time được nhập sẵn trong chương trình, các tham
số cần thiết của thuật toán sử dụng (giá trị tuỳ chọn)
- Output: In ra lần lượt với mỗi dòng gồm: process nào được chạy, thời điểm bắt đầu, thời gian chạy
của từng process
- Nộp bài kèm cả output được copy vào một file .txt
*/

//----------This is SJF Schedule------------

#include <iostream>
#include <fstream>
using namespace std;

int main()
{

	// Array for storing Process Id, Burst Time (Run time), Start Time of each process.
	int A[100][3];
	int i, j, n, index, temp;

	// Write to a file
	ofstream myfile;
	myfile.open("SJF_schedule.txt");

	cout << "Enter number of process: ";
	cin >> n;

	cout << "Enter Burst Time:" << endl;

	// User Input Burst Time and alloting Process Id.
	for (i = 0; i < n; i++)
	{
		cout << "Process" << i + 1 << ": ";
		// Input burst time
		cin >> A[i][1];
		// Allot Process id
		A[i][0] = i + 1;
	}

	// Sorting process according to their Burst Time.
	for (i = 0; i < n; i++)
	{
		index = i;
		for (j = i + 1; j < n; j++)
			if (A[j][1] < A[index][1])
				index = j;
		temp = A[i][1];
		A[i][1] = A[index][1];
		A[index][1] = temp;

		temp = A[i][0];
		A[i][0] = A[index][0];
		A[index][0] = temp;
	}

	A[0][2] = 0;
	// Calculation of Start Time of each Process 
	for (i = 1; i < n; i++)
	{
		A[i][2] = 0;
		for (j = 0; j < i; j++)
			A[i][2] += A[j][1];
	}

	cout << "<========SHORTEST JOB FIRST==============>" << endl;
	// Write to a file
	myfile<<"Process		Start_Time	Run_time" << endl;
	//Print to Terminal
	cout << "Process		Start_Time	Run_time" << endl;

	for (i = 0; i < n; i++)
	{
		// Printing the data to Terminal
		cout << "Process" << A[i][0] << "	" << A[i][2] << "	 	" << A[i][1] << endl;
		// Write to a file
		myfile << "Process" << A[i][0] << "	" << A[i][2] << "	 		" << A[i][1] << endl;
	}
	myfile.close();
}
