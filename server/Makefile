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
MYSQL_DEPS = obj/mysql_helper.o obj/mysql_test.o obj/json_helper.o obj/common.o
JSON_DEPS = obj/json_helper.o obj/json_test.o
CLIENT_EMULATOR_DEPS = obj/client_emulator.o obj/json_helper.o obj/client.o obj/common.o
ACCESS_LAYER_DEPS = obj/access_layer.o obj/json_helper.o obj/server.o obj/common.o obj/client.o
SERVICE_REIGSTER_DEPS = obj/service_register.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_LOGIN_DEPS = obj/service_login.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_LOGOFF_DEPS = obj/service_logoff.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_ADD_FRIEND_DEPS = obj/service_addFriend.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_CONFIRM_ADD_FRIEND_DEPS = obj/service_confirmAdd.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_GET_FRIENDS_DEPS = obj/service_getFriends.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o
SERVICE_RECEIVE_DEPS = obj/service_receiveMsg.o obj/mysql_helper.o obj/json_helper.o obj/server.o obj/common.o


#C_CALL_CPP_DEPS = obj/c_call_cpp_middle_ware.o
#C_CALL_CPP_DEPS_SO = bin/libc_call_cpp.so


debug : $(MYSQL_DEPS) $(JSON_DEPS) $(SERVICE_REIGSTER_DEPS) $(CLIENT_EMULATOR_DEPS) $(ACCESS_LAYER_DEPS) $(SERVICE_LOGIN_DEPS) $(SERVICE_LOGOFF_DEPS) $(SERVICE_ADD_FRIEND_DEPS) $(SERVICE_CONFIRM_ADD_FRIEND_DEPS) $(SERVICE_GET_FRIENDS_DEPS) $(SERVICE_RECEIVE_DEPS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(MYSQL_DEPS) -o bin/debug/MYSQL_TEST $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(JSON_DEPS) -o bin/debug/JSON_TEST
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(CLIENT_EMULATOR_DEPS) -o bin/debug/CLIENT_EMULATOR $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(ACCESS_LAYER_DEPS) -o bin/debug/ACCESS_LAYER $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_REIGSTER_DEPS) -o bin/debug/SERVICE_REIGSTER $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_LOGIN_DEPS) -o bin/debug/SERVICE_LOGIN $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_LOGOFF_DEPS) -o bin/debug/SERVICE_LOGOFF $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_ADD_FRIEND_DEPS) -o bin/debug/SERVICE_ADD_FRIEND $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_CONFIRM_ADD_FRIEND_DEPS) -o bin/debug/SERVICE_CONFIRM_ADD_FRIEND $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_GET_FRIENDS_DEPS) -o bin/debug/SERVICE_GET_FRIENDS $(MYSQL_FLAGS)
	$(CC) $(FLAGS) $(INCLUDEDIR)  -g $(SERVICE_RECEIVE_DEPS) -o bin/debug/SERVICE_RECEIVE $(MYSQL_FLAGS)
	

	

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

obj/common.o : src/common.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ -lpthread

obj/server.o : src/server.c
	gcc $(FLAGS) $(INCLUDEDIR) -g -c $< -o $@

obj/client.o : src/client.c
	gcc $(FLAGS) $(C_CALL_CPP_DEPS) $(INCLUDEDIR) -g -c $< -o $@

ojb/client_emulator.o : src/client_emulator.cpp
	gcc $(FLAGS) $(INCLUDEDIR) -g -c $< -o $@ -lpthread

obj/access_layer.o : src/access_layer.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_register.o : src/service_register.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_login.o : src/service_login.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_logoff.o : src/service_logoff.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_addFriend.o : src/service_addFriend.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_confirmAdd.o : src/service_confirmAdd.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_getFriends.o : src/service_getFriends.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/service_receiveMsg.o : src/service_receiveMsg.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS) -lpthread

obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@ $(MYSQL_FLAGS)

.PHONY : clean
clean :
	rm -f obj/*.o; rm -f obj/*o; rm -f bin/debug/*; rm -f bin/release/* 



