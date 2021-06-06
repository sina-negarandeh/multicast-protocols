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


void routerProcess(Router this_router);
void clientProcess(Client this_client);

class Network {
    public:
    Network();
    bool isCommandCorrect(std::string command_keyword,
    int command_arguments_number, std::string keyword, int argmenst_number);
    int handleCommand(std::string input);
    int run();

    int router(std::vector<std::string> &splitted_command);
    int findRouter(int router_id);
    int createNamePipe(std::string link);
    int connectRouter(std::vector<std::string> &splitted_command);
    int send(std::string cmd);

    private:
    std::vector<Router> routers_;
    std::vector<int> router_command_fd_;

};


#endif  // NETWORK_H_
