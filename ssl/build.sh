STATIC_LIBS="-lboost_system -lboost_filesystem -lssl -lcrypto -L./lib -lpthread -ldl -lrt"

g++ -o server server.cpp $STATIC_LIBS

g++ -o client client.cpp $STATIC_LIBS

