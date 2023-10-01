#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100

// Struct để lưu thông tin của mỗi contact
typedef struct {
    char name[50];
    char phone[15];
} Contact;

// Hàm để ghi toàn bộ danh bạ vào file
void writeContactsToFile(Contact contacts[], int numContacts, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi dữ liệu.\n", filename);
        return;
    }

    // Ghi số lượng contact vào phần header
    fwrite(&numContacts, sizeof(int), 1, file);
    /*
    Ghi index contact vao phan header
    
    */


    // Ghi danh sách contact vào phần dữ liệu
    for (int i = 0; i < numContacts; i++) {
        fwrite(&contacts[i], sizeof(Contact), 1, file);
    }

    fclose(file);
}

// Hàm để tìm contact theo tên và in ra số điện thoại từ file
void findContactAndPrintPhone(const char *name, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Không thể mở file %s để đọc dữ liệu.\n", filename);
        return;
    }

    int numContacts;
    fread(&numContacts, sizeof(int), 1, file);

    Contact contacts[MAX_CONTACTS];

    // Đọc danh sách contact từ phần dữ liệu
    for (int i = 0; i < numContacts; i++) {
        fread(&contacts[i], sizeof(Contact), 1, file);
    }

    // Tìm contact theo tên và in ra số điện thoại tương ứng
    int found = 0;
    for (int i = 0; i < numContacts; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            printf("Tên: %s\nSố điện thoại: %s\n", contacts[i].name, contacts[i].phone);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Không tìm thấy contact có tên '%s' trong danh bạ.\n", name);
    }

    fclose(file);
}

int main() {
    Contact contacts[MAX_CONTACTS];
    int numContacts = 0;
    
    // Điền thông tin danh bạ vào mảng contacts
    // Ví dụ: thêm hai contact vào danh bạ
    strcpy(contacts[0].name, "Nguyen Van A");
    strcpy(contacts[0].phone, "0987654321");
    numContacts++;

    strcpy(contacts[1].name, "Tran Thi B");
    strcpy(contacts[1].phone, "0123456789");
    numContacts++;

    // Ghi danh bạ ra file
    writeContactsToFile(contacts, numContacts, "danhba.txt");

    // Tìm contact theo tên và in ra số điện thoại
    findContactAndPrintPhone("Nguyen Van A", "danhba.txt");

    return 0;
}
