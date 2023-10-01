#khong chay duoc file
#khong lam dung yeu cau: Khong hien thi menu cac file rong
#point: 4

#!/bin/bash

# Kiểm tra xem có đủ tham số đầu vào hay không
if [ $# -ne 1 ]
then
  echo "Sử dụng: $0 [đường dẫn thư mục]"
  exit 1
fi

# Lưu đường dẫn thư mục vào biến dir_path
dir_path=$1

# Hàm để tìm và xóa các file có kích thước bằng 0 trong thư mục và các thư mục con
function delete_empty_files {
  for file in "$1"/*
  do
    if [ -d "$file" ]
    then
      delete_empty_files "$file" # Đệ quy vào thư mục con
    else
      if [ -s "$file" ]
      then
        continue # Bỏ qua nếu file có kích thước khác 0
      fi

      # In tên file và hỏi người dùng có muốn xóa không
      echo "File rỗng: $file"
      read -p "Bạn có muốn xóa file này không? (Y/N) " answer
      if [ "$answer" == "Y" ] || [ "$answer" == "y" ]
      then
        rm "$file" # Xóa file nếu người dùng chọn Y
        echo "Đã xóa $file"
      fi
    fi
  done
}

# Gọi hàm delete_empty_files với đường dẫn thư mục đã nhập
delete_empty_files "$dir_path"