#include <iostream>
#include <unordered_set>
#include <vector>
#include <climits>

using namespace std;

const int D = 16; // Kích thước của working set
const int numBits = 4; // Số bit để lưu trạng thái tham chiếu
const int referenceBitMask = 1 << (numBits - 1); // Mask để trích xuất bit cuối cùng (trạng thái tham chiếu)

void printWorkingSet(const unordered_set<int>& workingSet) {
    cout << "Working Set: ";
    for (int page : workingSet) {
        cout << page << " ";
    }
    cout << endl;
}

int main() {
    vector<int> pageAccessArray = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5}; // Chuỗi các page truy cập

    unordered_set<int> workingSet; // Khởi tạo working set

    // Khởi tạo thông tin về Working Set ban đầu
    cout << "Initial ";
    printWorkingSet(workingSet);

    // Xử lý truy cập các page
    for (int pageNumber : pageAccessArray) {
        // Kiểm tra nếu page không có trong working set
        if (workingSet.find(pageNumber) == workingSet.end()) {
            // Kiểm tra kích thước của working set
            if (workingSet.size() < D) {
                // Thêm page vào working set
                workingSet.insert(pageNumber);
            } else {
                // Tìm page có trạng thái tham chiếu (reference bit) nhỏ nhất để thay thế
                int pageToReplace = -1;
                int minReferenceBit = INT_MAX;

                for (int page : workingSet) {
                    int referenceBit = page & referenceBitMask;
                    if (referenceBit < minReferenceBit) {
                        minReferenceBit = referenceBit;
                        pageToReplace = page;
                    }
                }

                // Xóa page cần thay thế khỏi working set
                workingSet.erase(pageToReplace);

                // Thêm page mới vào working set
                workingSet.insert(pageNumber);
            }
        }

        // Cập nhật trạng thái tham chiếu của các trang trong working set
        for (int& page : workingSet) {
            if (page == pageNumber) {
                // Đặt trạng thái tham chiếu của trang vừa truy cập thành 1 (đã được tham chiếu)
                page |= referenceBitMask;
            } else {
                // Dịch trái trạng thái tham chiếu của trang
                page <<= 1;
            }
        }

        // In thông tin về page và working set
        cout << "Page " << pageNumber << ", ";
        printWorkingSet(workingSet);
    }

    return 0;
}
