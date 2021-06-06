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
    Client(std::string name, IP server_IP, IP router_IP, int router_port);
    void setIp(IP ip);
    void join(std::string group_name);
    void leave(std::string group_name);
    void select(std::string group_name);
    void sendFile(std::string file_name, std::string server_name);
    void sendMessage(std::string message, std::string server_name);
    void showGroup();
    void Sync();
    void SignOut();

    int send(std::string message, std::string link);
    int recieve();

    private:
    std::string name_;
    IP server_IP_;
    IP router_IP_;
    int router_port_;
    IP self_IP_;
    std::vector<std::string> joined_groups;
};


#endif