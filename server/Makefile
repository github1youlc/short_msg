CC = g++
FLAGS = -Wall
INCLUDEDIR = -I ./include
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
REGISTER_DEPS = mysql_helpler.o register_handler.o
MYSQL_DEPS = obj/debug/mysql_helper.o obj/debug/mysql_test.o
TARGET = mysql_test

debug : $(MYSQL_DEPS)
	$(cc) $(FLAGS) $(INCLUDEDIR)  -g $(MYSQL_DEPS) -o MSYQL_TEST 


obj/debug/mysql_helper.o : src/mysql_helper.cpp
	$(cc) $(FLAGS) $(INCLUDEDIR) -g $< -o $@ `mysql_config --cflags` `mysql_config --libs`


obj/debug/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -g $< -o $@

.PHONY : clean
clean :
	rm -f obj/*.o; rm -f bin/librec_call.so



