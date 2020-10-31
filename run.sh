#!/bin/bash

printf "Shell Script running...\n" && 
clear &&
g++ main.cpp ManageAllocation.cpp Object.cpp -o test -std=c++14 &&

# 空格必须存在
if [ ! -d out ]; then
    mkdir out
fi

mv test out/test &&
out/test &&

ret=$? # $? 显示最后命令的退出状态
printf "Program return $ret\n" 

# chmod u+x file # 赋予user执行权限 drwxrwxrwx
# d 表示目录