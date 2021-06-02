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
    Server(Ip server_IP);

    private:
    Ip server_IP_;
};


#endif