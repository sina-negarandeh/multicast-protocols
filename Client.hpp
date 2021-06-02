#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "utils.hpp"

class Client{
    public:
    Client(std::string name, Ip server_IP, Ip router_IP, int router_port);
    void setIp(Ip ip);
    void join(std::string group_name);
    void leave(std::string group_name);
    void select(std::string group_name);
    void sendFile(std::string file_name, std::string server_name);
    void sendMessage(std::string file_name, std::string server_name);
    void showGroup();
    void Sync();
    void SignOut();

    private:
    std::string name_;
    Ip server_IP_;
    Ip router_IP_;
    int router_port_;
    Ip self_IP_;
};


#endif