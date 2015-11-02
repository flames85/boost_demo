g++ -o server server.cpp -lboost_system -lboost_filesystem -lpthread -lssl -L${BOOSTDIR}/lib

g++ -o client client.cpp -lboost_system -lboost_filesystem -lpthread -lssl -L${BOOSTDIR}/lib

