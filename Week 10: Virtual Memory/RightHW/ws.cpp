#include <iostream>
#include <array>
const int bit = 4;

void updateWorkingSet(int arr[][bit], int* workingSet, int page) {
    // Kiểm tra xem có cột nào chứa ít nhất 1 số 1 hay không
    bool colContainsOnes[page];
    for (int i = 0; i<page;i++)
    {
        colContainsOnes[i]=0;
    }
    for (int col = 0; col < page; col++) {
        for (int row = 0; row < bit; row++) {
            if (arr[col][row] == 1) {
                colContainsOnes[col] = true;
                break;
            }
        }
    }

    // Cập nhật working set dựa trên các cột chứa ít nhất 1 số 1
    for (int col = 0; col < page; col++) {
        if (colContainsOnes[col]) {
            for (int row = 0; row < bit; row++) {
                if (arr[col][row] == 1) {
                    workingSet[col * bit + row] = col + 1; // Lưu số trang vào working set
                }
            }
        }
        else {
            for (int row = 0; row < bit; row++) {
                if (workingSet[col * bit + row] <= 4) { // Xóa page 1, 2, 3, 4 khỏi working set
                    workingSet[col * bit + row] = 0;
                }
            }
        }
    }
}

void printWorkingSet(int* workingSet, int array) {
    std::cout << "Working Set: ";
    for (int i = 0; i < array; i++) {
        if (workingSet[i] != 0) {
            std::cout << workingSet[i] << " ";
        }
    }
    std::cout << std::endl;
}

void printArray(int arr[][bit], int page) {
    std::cout << "Array: " << std::endl;
    for (int col = 0; col < page; col++) {
        for (int row = 0; row < bit; row++) {
            std::cout << arr[col][row] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    // Ví dụ về mảng page hoạt động (mảng T)
    int pageArray[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,18,19,20};

    int arraySize = sizeof(pageArray) / sizeof(pageArray[0]); // Số lượng phần tử trong pageArray
    int iterations = arraySize / 4; // Số lần lặp

    int max_page = pageArray[0];  // Giả sử phần tử đầu tiên là lớn nhất

    // Duyệt qua mảng để tìm giá trị lớn nhất
    for (int i = 1; i < arraySize; i++) {
        if (pageArray[i] > max_page) {
            max_page = pageArray[i];
        }
    }

    int page = max_page;
    int array = page * bit;

    int arr[page][bit];
    for (int i = 0; i<page;i++)
    {
        for (int j=0; j<bit;j++)
            arr[i][j]=0;
    }
    //  = {0};
    int workingSet[array];
    for (int i = 0; i<array;i++)
    {
        workingSet[i]=0;
    }
    //  = {0};

    for (int iteration = 0; iteration < iterations; iteration++) {
        // Đặt giá trị 0 cho tất cả các phần tử trong cột
        for (int row = 0; row < page; row++) {
            arr[row][iteration % bit] = 0;
        }

        // Lấy 4 phần tử mảng T tương ứng với cột hiện tại
        int startIndex = iteration * 4;
        for (int i = startIndex, j = 0; i < startIndex + 4; i++, j++) {
            int page = pageArray[i];
            arr[page - 1][iteration % bit] = 1; // Gán giá trị 1 cho ô tương ứng trong mảng 16*4
        }
        // In ra mảng 16*4
        std::cout << "After taking 4 pages from input: " << std::endl;
        printArray(arr, page);

        // Cập nhật working set và in ra kết quả
        updateWorkingSet(arr, workingSet, page);
        printWorkingSet(workingSet, array);
    }

    return 0;
}
