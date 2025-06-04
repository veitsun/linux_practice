#!/bin/bash

# 遍历当前目录下的所有 .cpp 文件
for file in *.cpp; do
    # 获取不带扩展名的文件名
    filename="${file%.cpp}"
    
    # 使用 g++ 编译成可执行文件
    g++ "$file" -o ./app/"$filename" 
    
    # 检查编译是否成功
    if [ $? -eq 0 ]; then
        echo "Compiled $file to $filename"
    else
        echo "Failed to compile $file"
    fi
done