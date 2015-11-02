STATIC_LIBS=-lboost_system -lboost_filesystem -lssl -lcrypto -L./lib -lpthread -ldl -lrt

all : client server

client : client.cpp
	g++ -g -o client client.cpp ${STATIC_LIBS}
server : server.cpp
	g++ -g -o server server.cpp ${STATIC_LIBS}

clean :
	rm client server
