#include "Client.hpp"

using namespace std;

Client::Client(std::string name, IP server_IP, IP router_IP, int router_port): server_IP_(server_IP), router_IP_(router_IP), self_IP_(0,0,0,0){
    name_ = name;
    router_port_ = router_port;
}

void Client::setIp(IP ip){
    self_IP_ = ip;
}