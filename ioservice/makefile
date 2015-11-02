all : ioservice

ioservice : ioservice.cpp
	g++ -g -o ioservice ioservice.cpp -lpthread -lboost_system -L${BOOSTDIR}/lib
#g++ -o ioservice ioservice.cpp -lpthread -lboost_system -L${BOOSTDIR}/lib -I${BOOSTDIR}/include

clean :
	rm  ioservice
