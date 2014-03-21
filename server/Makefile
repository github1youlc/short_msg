CC = g++
FLAGS = -Wall
INCLUDEDIR = -I ./include
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
REGISTER_DEPS = mysql_helpler.o register_handler.o
MYSQL_DEPS = mysql_helper.o mysql_test.o
TARGET = mysql_test

all : $(MYSQL_DEPS)
	$(cc) $(FLAGS) $(INCLUDEDIR) -o MSYQL_TEST 


obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@

.PHONY : clean
clean :
	rm -f obj/*.o; rm -f bin/librec_call.so



