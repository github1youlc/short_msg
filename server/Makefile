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
JSON_DEPS = obj/json_helper.o obj/json_test.o
REGISTER_SERVER_DEPS = obj/register_server.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
CLIENT_EMULATOR_DEPS = obj/client_emulator.o obj/json_helper.o obj/client.o
#C_CALL_CPP_DEPS = obj/c_call_cpp_middle_ware.o
#C_CALL_CPP_DEPS_SO = bin/libc_call_cpp.so

debug : $(C_CALL_CPP_DEPS_SO) $(MYSQL_DEPS) $(JSON_DEPS) $(REGISTER_SERVER_DEPS) $(CLIENT_EMULATOR_DEPS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(MYSQL_DEPS) -o bin/debug/MYSQL_TEST $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(JSON_DEPS) -o bin/debug/JSON_TEST
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(REGISTER_SERVER_DEPS) -o bin/debug/REGISTER_SERVER $(MYSQL_FLAGS) 
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(CLIENT_EMULATOR_DEPS) -o bin/debug/CLIENT_EMULATOR
	

release : $(MYSQL_DEPS)
	$(CC) $(FLAGS) $(INCLUDEDIR) -O2 $(MYSQL_DEPS) -o bin/release/MYSQL_TEST $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR) -O2 $(MYSQL_DEPS) -o bin/release/JSON_TEST $(MYSQL_FLAGS)


obj/mysql_helper.o : src/mysql_helper.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

obj/mysql_test.o : test/mysql_test.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

obj/json_helper.o : src/json_helper.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

obj/json_test.o : test/json_test.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

obj/register_server.o : src/register_server.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/common.o : src/common.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/server.o : src/server.c
	gcc $(FLAGS) $(INCLUDEDIR) -g -c $< -o $@

obj/client.o : src/client.c
	gcc $(FLAGS) $(C_CALL_CPP_DEPS) $(INCLUDEDIR) -g -c $< -o $@

ojb/client_emulator.o : src/client_emulator.cpp
	gcc $(FLAGS) $(INCLUDEDIR) -g -c $< -o $@

obj/c_call_cpp_middle_ware.o : src/c_call_cpp_middle_ware.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@

$(C_CALL_CPP_DEPS_SO) : obj/c_call_cpp_middle_ware.o
	$(CC) $(FLAGS) -shared -fPIC $(INCLUDEDIR) $< -o $@

obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

.PHONY : clean
clean :
	rm -f obj/*.o; rm -f obj/*o; rm -f bin/debug/*; rm -f bin/release/* 



