#ifndef ROUTER_H_
#define ROUTER_H_

#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#include <vector>
#include <string>
#include <iostream>

#define DEFAULT_PORTS_NUMBER 10

class Router {
    public:
    Router(int number_of_ports, int id, int command_fd);
    int getID();
    int getCommandFd();
    int shutdown();
    int requestConnect(int router_id, int port_number);
    int acceptConnect(int router_id, int port_number);
    int writeMessage(std::string link, std::string message, int router_id);
    int readMessage(std::string link, char* message, size_t message_size, int router_id);

    private:
    size_t number_of_ports_;
    int id_;
    int command_fd_;
};


#endif  // ROUTER_H_
