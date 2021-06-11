#ifndef _SERVER_HPP
#define _SERVER_HPP

#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "utils.hpp"

class Server{
    public:
    Server(IP server_IP, IP router_IP, int router_port, int command_fd);
    int getID(){ return id_; }
    IP getIP(){ return server_IP_; }

    void setCommandFd(int command_fd){ command_fd_ = command_fd; }
    int getCommandFd(){ return command_fd_; }

    private:
    int command_fd_;
    IP server_IP_;
    IP router_IP_;
    int router_port_;
    int id_=666;
};


#endif