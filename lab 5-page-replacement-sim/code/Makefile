# 定义头文件、源文件和编译参数
HEAD = vmrp.h
SRC = vmrp.cc
OBJ = vmrp.o
CXX = g++
CXXFLAGS = -Wall -g -c  # 添加警告和调试信息

# 默认目标：生成可执行文件
all: vmrp

# 生成可执行文件
vmrp: $(OBJ)
    $(CXX) $(OBJ) -o vmrp

# 编译源文件为对象文件
$(OBJ): $(SRC) $(HEAD)
    $(CXX) $(CXXFLAGS) $(SRC)

# 清理生成的文件
clean:
    rm -f *.o vmrp