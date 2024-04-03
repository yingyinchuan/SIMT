# 编译器设置
CXX = g++
# 编译选项
CXXFLAGS = -Wall -Wextra -std=c++11
# 目标文件
TARGET = program
# 源文件
SRCS = main.cc
# 目标文件依赖于源文件
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)
	rm -f test.myzip unzip.txt

test:
	./program
