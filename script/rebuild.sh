#!/bin/bash

# 检查是否传递了 "rebuild" 参数

cd "$(dirname "$0")/.."
pwd

if [ -d "build" ]; then
rm -rf build
echo "Build directory removed."
fi
mkdir build
echo "Build directory created."

# 进入 build 目录并运行 cmake 和 make
cd build &&
cmake .. &&
make &&
echo -e "\033[33m\nCMake and Make completed.\n\033[0m" &&

# 运行 ctest
ctest --test-dir tests --output-on-failure&&
echo -e "\033[33m\nCTest completed.\n\033[0m"

# 查找第一个可执行文件
default_executable=$(find . -type f -perm -111 | head -n 1)

# 用户确认是否运行可执行文件
read -p "Do you want to run the executable? (y/n): " -n 1 -r
echo    # 移到新行
if [[ $REPLY =~ ^[Yy]$ ]] || [[ -z $REPLY ]]; then
  if [ -n "$1" ]; then
    executable=./"$1"
  else
    executable="$default_executable"
  fi

  if [ -n "$executable" ]; then
      echo "Executable specified: $executable"
      if [ -f "$executable" ]; then
          echo "Executable found at: $executable"
          $executable
          echo -e "\033[32mExecutable $executable run completed.\033[0m"
      else
          echo -e "\033[31mExecutable $executable not found at: $executable\033[0m"
      fi
  else
      echo -e "\033[31mNo executable file specified.\033[0m"
  fi
else
  echo -e "\033[33mExecutable not run.\033[0m"
fi