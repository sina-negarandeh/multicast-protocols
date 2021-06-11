#include "Server.hpp"

using namespace std;

Server::Server(IP server_IP, IP router_IP, int router_port, int command_fd) : server_IP_(server_IP), router_IP_(router_IP){
    router_port_ = router_port;
    command_fd_ = command_fd;
}

