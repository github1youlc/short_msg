CC = g++
FLAGS = -Wall -o
INCLUDEDIR = -I ./include
CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/, $(notdir $(CPP_FILES:.cpp=.o)))
REGISTER_DEPS = mysql_helpler.o register_handler.o 
TARGET = register_handler login_handler

all : $(TARGET)

register_handler : $(REGISTER_DEPS) 
	$(cc) $(FLAGS) $(INCLUDEDIR) $< -o $@

obj/%.o : src/%.cpp
	$(CC) $(FLAGS) $(INCLUDEDIR) -c $< -o $@

.PHONY : clean
clean :
	rm -f obj/*.o; rm -f bin/librec_call.so



