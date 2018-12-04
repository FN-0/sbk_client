# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = g++
CFLAGS  := -Wall

# opencv 头文件和lib路径 
OPENCV_INC_ROOT = /usr/local/include
JNI_INC_ROOT = /usr/lib/jvm/java-11-openjdk-amd64/include
JNI_INC_ROOT_LINUX = /usr/lib/jvm/java-11-openjdk-amd64/include/linux
OPENCV_LIB_ROOT = /usr/local/lib
# C文件
SRCS = TestNative.cpp 
# 目标文件 
OBJS = $(SRCS:.cpp=.o)
LIB = libtestnative.so # 目标文件名 

OPENCV_INC= -I $(OPENCV_INC_ROOT) -I $(JNI_INC_ROOT) -I $(JNI_INC_ROOT_LINUX)

INCLUDE_PATH = $(OPENCV_INC)

LIB_PATH = -L $(OPENCV_LIB_ROOT)

# 依赖的lib名称
OPENCV_LIB = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_ml  

all : $(LIB)

# 2. 生成.o文件 
%.o : %.cpp
	$(CC) $(CFLAGS) -fpic -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OPENCV_LIB) 

# 3. 生成动态库文件
$(LIB) : $(OBJS)
	rm -f $@
	g++ -shared -o $@ $(OBJS) $(LIB_PATH) $(OPENCV_LIB)
	rm -f $(OBJS)

tags :
	ctags -R *

# 4. 删除中间过程生成的文件 
clean:
	rm -f $(OBJS) $(TARGET) $(LIB)
