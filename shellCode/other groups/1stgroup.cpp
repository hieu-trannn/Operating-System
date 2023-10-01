//lam sai yeu cau cua thay: Shell Script
// neu nhap lua chon: cac ki tu khac, khong phai 1, 2 thi se bi loi
// van chay duoc file, xoa duoc file, hien thi menu,...
//Point: 5

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void print_menu(char *filename);

int main(int argc, char *argv[]) {
    // Kiem tra so luong tham so dau vao
    if (argc < 2) {
        printf("Khong du tham so dau vao.\n");
        return 1;
    }
    
    // Mo thu muc v� kiem tra
    DIR *dir;
    struct dirent *entry;
    struct stat filestat;
    
    dir = opendir(argv[1]);
    if (!dir) {
        printf("Khong the mo thu muc.\n");
        return 1;
    }
    
    // In ra c�c file c� kich thuoc l� 0 trong thu muc v� c�c thu muc con
    while ((entry = readdir(dir)) != NULL) {
        // Lay thong tin ve file hoac thu muc
        char filename[512];
        snprintf(filename, sizeof(filename), "%s/%s", argv[1], entry->d_name);
        if (stat(filename, &filestat)) {
            continue;
        }
        
        // Kiem tra xem neu do l� file c� kich thuoc l� 0 th� in ra v� hoi nguoi dung c� muon xoa kh�ng
        if (S_ISREG(filestat.st_mode) && filestat.st_size == 0) {
            printf("File %s co kich thuoc l� 0.\n", filename);
            print_menu(filename);
        } else if (S_ISDIR(filestat.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            // Neu do l� thu muc th� tiep tuc de quy vao thu muc do
            char *newdirpath = (char *) malloc(strlen(filename) + 1);
            strcpy(newdirpath, filename);
            char* args[] = { argv[0], newdirpath };
			main(2, args);
            free(newdirpath);
        }
        
    }
    
    // �ong thu muc
    closedir(dir);
    
    return 0;
}

void print_menu(char *filename) {
    // In menu v� hoi nguoi dung c� muon xoa file khong
    int choice;
    printf("Ban co muon xoa file %s khong?\n", filename);
    printf("1. Co\n");
    printf("2. Khong\n");
    printf("Nhap lua chon cua ban: ");
    scanf("%d", &choice);
    
    if (choice == 1) {
        // Xoa file neu nguoi d�ng chon c�
        if (remove(filename) != 0) {
            printf("Khong the xoa file.\n");
        } else {
            printf("File da duoc xoa.\n");
        }
    } else {
        printf("File khong duoc xoa.\n");
    }
}

