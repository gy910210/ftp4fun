# Makefile for the socket programming example
#

server_objects = ServerSocket.o Socket.o server_main.o
client_objects = ClientSocket.o Socket.o client_main.o Command.o

all : ftps ftpc

ftps: $(server_objects)
	g++ -o ftps $(server_objects) -lpthread

ftpc: $(client_objects)
	g++ -o ftpc $(client_objects) -lpthread

Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
User: User.cpp
Command: Command.cpp

server_main: server_main.cpp
	g++ -o server_main.cpp -lpthread
client_main: client_main.cpp
	g++ -o client_main.cpp -lpthread


.PHONY : clean
clean:
	rm -f *.o ftps ftpc
