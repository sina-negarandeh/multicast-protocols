#ifndef _GROUP_SERVER_HPP
#define _GROUP_SERVER_HPP

#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "utils.hpp"

class GroupServer{
    public:
    GroupServer(int listen_port);

    private:
    Ip server_IP_;
    int listen_port_;
};


#endif