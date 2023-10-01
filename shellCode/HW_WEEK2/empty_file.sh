#!/bin.bash

#PLEASE READ README.MD FIRST!!!

#Compile file menu.c, get execution file: menu
gcc menu.c -o menu

#Command-line-argument: Path to the folder
path=$@;

#get empty files in folder
empty_files=$(find $path -type f -empty)

# show empty file with execution of menu.cpp file (show menu of empty files and give user option: delete or not) 
# p: only show menu 
./menu p $empty_files

# ask user delete empty file or not
for item in $empty_files; do
    echo "===================================="
    echo "Do you wanna delete $item: "
    # n: show menu and give option: delete file or not
    ./menu n $item
    # $# receive return output of execution file: menu
    if [ $? = "0" ]; then
        #delete file
        rm -f $item
        echo "Delete $item successfully"
    else
        echo "===================================="
        echo "Keep $item"
    fi
done

#announce: run successfully
echo "===================================="
echo "Mission Complete"



