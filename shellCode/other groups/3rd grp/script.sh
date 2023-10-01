#chay duoc, dung yeu cau cua thay: hien thi menu file rong
#chua lam duoc tham so dong lenh: duong dan
#point: 8
#!/bin/bash
gcc menu.c -o menu
mapfile -t FILES < <(find $1 -size 0 -type f)

declare  i=0
while [ $i -lt $[${#FILES[@]}+1] ]
	do
		files=$(printf " %s" "${FILES[@]:$i}")
		files=${files:1}
		./menu $files
		if [ $? -eq 1 ]; then
			rm ${FILES[$i]}
			echo ${FILES[$i]} deleted!
		else 
			echo "Another one?"
		fi
		i=$[$i+1]
	done

rm menu