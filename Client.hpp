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
    Client(std::string name, int id, IP server_IP, IP router_IP, int router_port, int command_fd);
    void setIP(IP ip);
    int getID();
    std::string getIP();
    void join(std::string group_name);
    void leave(std::string group_name);
    void select(std::string group_name);
    void sendFile(std::string file_name, std::string server_name);
    void sendMessage(std::string message, std::string server_name);
    void showGroup();
    void Sync();
    void SignOut();

    int requestConnect(int router_id, int port_number);
    int writeMessage(std::string link, std::string message, int router_id);
    int readMessage(std::string link, char* message, size_t message_size, int router_id);

    void setCommandFd(int command_fd){ command_fd_ = command_fd; }
    int getCommandFd(){ return command_fd_; }
    std::string getName(){ return name_; }

    int send(std::string message);
    int recieve();

    private:
    std::string name_;
    int id_;
    IP server_IP_;
    IP router_IP_;
    int router_port_;
    IP self_IP_;
    int command_fd_;
    std::vector<std::string> joined_groups;
    std::string link_;
};

#endif
