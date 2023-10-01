#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int D = 16; // Số frame tối đa sử dụng

struct Page {
    int pageNumber;
    int referenceNumber;
};

void printWorkingSet(const vector<Page>& workingSet) {
    cout << "Working Set: ";
    for (const Page& page : workingSet) {
        cout << page.pageNumber << "(" << page.referenceNumber << ") ";
    }
    cout << endl;
}

void printReferenceNumbers(const vector<Page>& pages) {
    cout << "Reference Numbers: ";
    for (const Page& page : pages) {
        cout << page.referenceNumber << " ";
    }
    cout << endl;
}

int main() {
    vector<int> pageAccessArray = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5, 1, 2, 3, 4, 5}; // Chuỗi các page truy cập

    vector<Page> workingSet; // Khởi tạo working set
    vector<Page> pages(D); // Khởi tạo mảng các trang

    // Khởi tạo thông tin về Working Set và reference numbers ban đầu
    cout << "Initial ";
    printWorkingSet(workingSet);
    printReferenceNumbers(pages);

    // Xử lý truy cập các page
    for (int pageNumber : pageAccessArray) {
        // Cập nhật reference number cho các trang
        for (Page& page : pages) {
            if (page.pageNumber == pageNumber) {
                page.referenceNumber++;
            }
        }

        // Kiểm tra nếu page không có trong working set
        bool found = false;
        for (Page& page : workingSet) {
            if (page.pageNumber == pageNumber) {
                found = true;
                break;
            }
        }

        if (!found) {
            // Kiểm tra kích thước của working set
            if (workingSet.size() < D) {
                // Thêm page vào working set và cập nhật reference number
                Page page;
                page.pageNumber = pageNumber;
                page.referenceNumber = 1;
                workingSet.push_back(page);
            } else {
                // Tìm page có reference number nhỏ nhất để thay thế
                int minReferenceNumber = 30000;
                int minIndex = -1;

                for (int i = 0; i < workingSet.size(); i++) {
                    if (workingSet[i].referenceNumber < minReferenceNumber) {
                        minReferenceNumber = workingSet[i].referenceNumber;
                        minIndex = i;
                    }
                }

                // Thay thế page có reference number nhỏ nhất bằng page mới
                workingSet[minIndex].pageNumber = pageNumber;
                workingSet[minIndex].referenceNumber = 1;
            }
        }

        // In thông tin về page, reference numbers và working set
        cout << "Page " << pageNumber << ", ";
        printReferenceNumbers(pages);
        printWorkingSet(workingSet);
    }

    return 0;
}
