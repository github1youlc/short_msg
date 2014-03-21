CFLAGS=-g -lpthread -lmysqlclient
SRC=../src
TEST=.
OBJ=../obj
BIN=../bin
INCLUDE=../include
all: build
	echo "Build successfully!"

run: 
	#${TARGET}/server
	#${TARGET}/client
	${TARGET}/sql_api
build : server.o client.o testServer.o testClient.o
	#cc ${CFLAGS} -o ${TARGET}/server \
	#	${TARGET}/server.o
	cc ${CFLAGS} -o ${BIN}/testServer \
		${OBJ}/testServer.o ${OBJ}/server.o
	#cc ${CFLAGS} -o ${TARGET}/client \
	#	${TARGET}/client.o
	cc ${CFLAGS} -o ${BIN}/testClient \
		${OBJ}/testClient.o ${OBJ}/client.o
	
rebuild: clean build
	echo rebuilded!
 
server.o : ${SRC}/server.c ${INCLUDE}/socketConnect.h
	cc ${CFLAGS} -o ${OBJ}/server.o -c ${SRC}/server.c
	
testServer.o: server.o ${INCLUDE}/socketConnect.h ${TEST}/testServer.c
	cc ${CFLAGS} -o ${OBJ}/testServer.o -c ${TEST}/testServer.c 
	
client.o : ${SRC}/client.c ${INCLUDE}/socketConnect.h
	cc ${CFLAGS} -o ${OBJ}/client.o -c ${SRC}/client.c
	
testClient.o: client.o ${INCLUDE}/socketConnect.h ${TEST}/testClient.c
	cc ${CFLAGS} -o ${OBJ}/testClient.o -c ${TEST}/testClient.c
	
clean :
	rm  -rf target/*