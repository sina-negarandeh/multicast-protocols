#include "GroupServer.hpp"

using namespace std;

GroupServer::GroupServer(int listen_port) : server_IP_(0,0,0,0){
    listen_port_ = listen_port;
}