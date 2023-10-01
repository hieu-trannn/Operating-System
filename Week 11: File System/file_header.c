#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 100

// Struct để lưu thông tin của mỗi contact
typedef struct {
    char name[50];
    char phone[15];
} Contact;

// Struct để lưu thông tin của mỗi phần tử header
typedef struct {
    char name[50];
    int logicalAddress;
} HeaderElement;

// Hàm để ghi toàn bộ danh bạ vào file
void writeContactsToFile(Contact contacts[], int numContacts, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Không thể mở file %s để ghi dữ liệu.\n", filename);
        return;
    }

    // Ghi phần header vào file
    HeaderElement header[MAX_CONTACTS];
    for (int i = 0; i < numContacts; i++) {
        strcpy(header[i].name, contacts[i].name);
        header[i].logicalAddress = i;
    }
    fwrite(header, sizeof(HeaderElement), MAX_CONTACTS, file);

    // Ghi danh sách contact vào phần dữ liệu
    for (int i = 0; i < numContacts; i++) {
        fwrite(&contacts[i], sizeof(Contact), 1, file);
    }

    fclose(file);
}

// Hàm để tìm contact theo tên và in ra số điện thoại từ file thông qua việc tra cứu phần header
void findContactAndPrintPhone(const char *name, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Không thể mở file %s để đọc dữ liệu.\n", filename);
        return;
    }

    // Đọc phần header từ file
    HeaderElement header[MAX_CONTACTS];
    fread(header, sizeof(HeaderElement), MAX_CONTACTS, file);

    // Tìm contact theo tên trong phần header
    int found = 0;
    for (int i = 0; i < MAX_CONTACTS; i++) {
        if (strcmp(header[i].name, name) == 0) {
            // Đọc contact tương ứng với logical address từ phần dữ liệu
            Contact contact;
            fseek(file, sizeof(Contact) * header[i].logicalAddress + sizeof(HeaderElement) * MAX_CONTACTS, SEEK_SET);
            fread(&contact, sizeof(Contact), 1, file);

            printf("Tên: %s\nSố điện thoại: %s\n", contact.name, contact.phone);
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
    writeContactsToFile(contacts, numContacts, "danhba.bin");

    // Tìm contact theo tên và in ra số điện thoại
    findContactAndPrintPhone("Nguyen Van A", "danhba.bin");

    return 0;
}
