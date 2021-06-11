#ifndef ROUTER_H_
#define ROUTER_H_

#include <string.h>
#include <unistd.h>
#include <fcntl.h> 

#include <vector>
#include <string>
#include <iostream>

#include "utils.hpp"

#define DEFAULT_PORTS_NUMBER 10

class Router {
    public:
    Router(std::string router_IP_address, int number_of_ports, int id, int command_fd);
    //Router(int number_of_ports, int id, int command_fd);
    int getID();
    int getCommandFd();
    std::string getIP();
    int shutdown();
    int requestConnect(int router_id, int port_number);
    int acceptConnect(int router_id, int port_number, int device_type);
    int writeMessage(std::string link, std::string message, int router_id);
    int readMessage(std::string link, char* message, size_t message_size, int router_id);
    bool hasInLookupTable(DeviceInfo device_info);
    int updateLookupTable(DeviceInfo device_info);
    void printLookupTable();
    std::vector<DeviceInfo> getLookupTable();
    int updateLookupTable(std::vector<DeviceInfo> lookup_table, std::string IP);
    void receive();
    

    private:
    std::string router_IP_address_;
    size_t number_of_ports_;
    int id_;
    int command_fd_;
    std::vector<DeviceInfo> lookup_table_;
    std::string link_;
    std::vector<DeviceInfo> connected_routers_;
};


#endif  // ROUTER_H_
