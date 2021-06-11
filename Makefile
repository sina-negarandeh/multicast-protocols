CC := g++ -std=c++17

all : network

network : main.o network.o router.o utils.o Packet.o Client.o Server.o
		$(CC) main.o network.o router.o utils.o Packet.o Client.o Server.o -o network

main.o : main.cpp network.hpp router.hpp utils.hpp
	$(CC) -c main.cpp

router.o : router.cpp router.hpp utils.hpp
	$(CC) -c router.cpp

utils.o : utils.cpp utils.hpp
	$(CC) -c utils.cpp

Packet.o : Packet.cpp Packet.hpp
	$(CC) -c Packet.cpp

Client.o : Client.cpp Client.hpp
	$(CC) -c Client.cpp

Server.o : Server.cpp Server.hpp
	$(CC) -c Server.cpp

clean:
	rm *.o network r_link_* w_link_*
