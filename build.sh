#!/bin/bash
sudo apt update -y
apt install -y git
# 创建3rdparty目录
mkdir -p 3rdparty

# 进入3rdparty目录
cd 3rdparty

# 检查ylib是否已经克隆
if [ ! -d "ylib" ]; then
    # 克隆ylib C++开发库
    git clone https://github.com/Liuccysdgg/ylib.git

   
else
    echo "ylib exist"
fi
 # 进入ylib目录
cd ylib
chmod 777 build.sh
# 调用buildl.sh进行编译
./build.sh

# 返回3rdparty目录
cd ..
# 返回最初的目录
cd ..

# 克隆fastweb仓库
if [ ! -d "fastweb" ]; then
    git clone https://github.com/Liuccysdgg/fastweb.git
fi

# 进入fastweb目录
cd fastweb

# 创建build目录
mkdir -p build

# 进入build目录
cd build

# 使用cmake配置fastweb代码
cmake ..

# 编译fastweb
make
make install
