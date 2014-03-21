#	Author: youlc
#	Email: ylcaa@foxmail.com
#	Note:
#		Deps: mysql-devel, boost library
#
CC = g++
FLAGS = -Wall
MYSQL_FLAGS = `mysql_config --cflags` `mysql_config --libs`
INCLUDEDIR = -I ./include
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
REGISTER_DEPS = mysql_helpler.o register_handler.o
MYSQL_DEPS = obj/mysql_helper.o obj/mysql_test.o

debug : $(MYSQL_DEPS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(MYSQL_DEPS) -o bin/debug/MYSQL_TEST $(MYSQL_FLAGS)


release : $(MYSQL_DEPS)
	$(CC) $(FLAGS) $(INCLUDEDIR) -O2 $(MYSQL_DEPS) -o bin/release/MYSQL_TEST $(MYSQL_FLAGS)


obj/mysql_helper.o : src/mysql_helper.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)


obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

.PHONY : clean
clean :
	rm -f obj/debug/*.o; rm -f obj/release/*o; rm -f bin/* 



