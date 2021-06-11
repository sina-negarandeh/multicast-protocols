#ifndef NETWORK_H_
#define NETWORK_H_

#include <string.h>
#include <sys/stat.h>

#include <vector>
#include <string>
#include <iostream>

#include "router.hpp"
#include "utils.hpp"
#include "Client.hpp"
#include "Packet.hpp"
#include "Server.hpp"


void routerProcess(Router this_router);
void clientProcess(Client this_client);
void serverProcess(Server this_server);

class Network {
    public:
    Network();
    bool isCommandCorrect(std::string command_keyword,
    int command_arguments_number, std::string keyword, int argmenst_number);
    int handleCommand(std::string input);
    int run();

    int client(std::vector<std::string> &splitted_command);
    int findClient(int client_id);
    int connectClient(DeviceInfo device_info, std::string router_IP);

    int server(std::vector<std::string> &splitted_command);
    int getMainServer();
    int connectServer(DeviceInfo device_info, std::string router_IP);

    int router(std::vector<std::string> &splitted_command);
    int findRouter(int router_id);
    int createNamePipe(std::string link);
    int connectRouter(std::vector<std::string> &splitted_command);
    int send(std::string cmd);

    int updateLookupTable(int router_index, DeviceInfo device_info);
    void printLookupTable();

    private:
    std::vector<Server> servers_;
    std::vector<Client> clients_;
    std::vector<Router> routers_;
    int server_command_fd_;
    std::vector<int> router_command_fd_;
    std::vector<int> client_command_fd_;
    std::vector<std::vector<int> > connected_routers_;
    std::vector<std::vector<int> > client_connected_routers_;

};


#endif  // NETWORK_H_
