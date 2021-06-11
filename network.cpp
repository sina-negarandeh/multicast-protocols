#include "network.hpp"
#include <exception>

using namespace std;

Network::Network() {

}

int Network::run() {
    string command;
    while (getline(cin, command)) {
        if (handleCommand(command))
            cout << "ERROR: Unknown command." << endl;
    }
    return 0;
}

bool Network::isCommandCorrect(string command_keyword,
    int command_arguments_number, string keyword, int argmenst_number) {
    if (command_keyword == keyword) {
        if (argmenst_number < command_arguments_number) {
            cout << "ERROR: Not enough arguments." << endl;
            return false;
        } else if (argmenst_number > command_arguments_number) {
            cout << "ERROR: Too many arguments." << endl;
            return false;
        }
    } else {
        return false;
    }
    
    return true;
}

int Network::handleCommand(string command) {
    vector<string> splitted_command = splitCommand(command);

    if (splitted_command.size() == 0) {
        return 0;
    }
    
    string command_keyword = splitted_command[0];
    try {
        if (isCommandCorrect("Router", 3, command_keyword, splitted_command.size())) {
            this->router(splitted_command);
            return 0;
        } else if (isCommandCorrect("RouterConnect", 5, command_keyword, splitted_command.size())) {
            this->connectRouter(splitted_command);
            return 0;
        } else if (isCommandCorrect("ChangeLinkCost", 3, command_keyword, splitted_command.size())) {

        } else if (isCommandCorrect("DisconnectLink", 2, command_keyword, splitted_command.size())) {

        } else if (isCommandCorrect("RouterShow", 2, command_keyword, splitted_command.size())) {

        } else if (isCommandCorrect("PrintLookupTable", 1, command_keyword, splitted_command.size())) {
            this->printLookupTable();
            return 0;
        } else if (isCommandCorrect("Client", 6, command_keyword, splitted_command.size())) {
            this->client(splitted_command);
            return 0;
        } else if (isCommandCorrect("Server", 4, command_keyword, splitted_command.size())) {
            this->server(splitted_command);
            return 0;
        } else {
            return 1;
        }
    } catch (exception &error) {
        cout << "Exception: " << error.what() << endl;
    }

    return 0;
}

void clientProcess(Client this_client) {
    int read_fd = this_client.getCommandFd();

    int flags = fcntl(read_fd, F_GETFL, 0);
    fcntl(read_fd, F_SETFL, flags | O_NONBLOCK);

    size_t message_size = 100;
    char message[message_size];

    while (true) {
        int read_bytes = read(read_fd, message, message_size);
        if (read_bytes <= 0) {
            continue;
        }

        cout << "Client " << this_client.getName() << ": Read " << read_bytes << " bytes. The message is: " << message << endl;
        vector<string> splitted_command = splitCommand(message);
        string command_keyword = splitted_command[0];
        //TODO: complete!
        if (command_keyword == "connect") {
            int router_number = stoi(splitted_command[1]);
            int port_number = stoi(splitted_command[2]);
            //this_client.connect();
        }
    }
}

int Network::server(std::vector<std::string> &splitted_command){
    int fds[2];
    if (pipe(fds) < 0) {
        cout << "Network: Failed to create pipe." << endl;
    }

    int read_fd = fds[READ];
    int write_fd = fds[WRITE];

    IP server_IP(splitted_command[1]);
    IP router_IP(splitted_command[2]);
    int router_port = stoi(splitted_command[3]);
    int server_id = 666;

    servers_.push_back(Server(server_IP, router_IP, router_port, read_fd));

    server_command_fd_ =  write_fd;

    DeviceInfo device_info;
    device_info.id_ = server_id;
    device_info.IP_address_ = server_IP.get_string();
    device_info.port_number_ = router_port;
    device_info.type = SERVER;
    connectServer(device_info, router_IP.get_string());
    
    int pid = fork();
    if (pid == -1) {
        cout<<"error: something's wrong, fork failed"<<endl;
        exit(-1);
    }else if (pid == 0) {
        usleep(10000);
        serverProcess(servers_[0]);
        exit(-1);
    }

    string message = "Hello from Network!";

    if (write(write_fd, message.c_str(), strlen(message.c_str()) + 1) < 0) {
       cout << "Network: Faile to write to system " << routers_[routers_.size() - 1].getID() << " command file descriptor." << endl;
    }

    return 1;
}

int Network::getMainServer(){
    return servers_[0].getID();
}

int Network::connectServer(DeviceInfo device_info, std::string router_IP){
    for (int i = 0; i < routers_.size(); i++) {
        if (routers_[i].getIP() == router_IP) {
            this->updateLookupTable(i, device_info);
        }
    }
}


void serverProcess(Server this_server){
    //TODO: complete
}

int Network::client(std::vector<std::string> &splitted_command) {
    int fds[2];
    if (pipe(fds) < 0) {
        cout << "Network: Failed to create pipe." << endl;
    }

    int read_fd = fds[READ];
    int write_fd = fds[WRITE];

    int client_id = stoi(splitted_command[2]);
    IP server_IP(splitted_command[3]);
    IP router_IP(splitted_command[4]);

    clients_.push_back(Client(splitted_command[1], client_id, server_IP, router_IP, stoi(splitted_command[5]), read_fd));
    this->client_command_fd_.push_back(write_fd);

    DeviceInfo device_info;
    device_info.id_ = client_id;
    device_info.IP_address_ = router_IP.get_string();
    device_info.port_number_ = stoi(splitted_command[4]);
    device_info.type = SYSTEM;
    connectClient(device_info, router_IP.get_string());
    
    int pid = fork();
    if (pid == -1) {
        cout<<"error: something's wrong, fork failed"<<endl;
        exit(-1);
    }else if (pid == 0) {
        usleep(10000);
        clientProcess(clients_[clients_.size() - 1]);
        exit(-1);
    }

    string message = "Hello from Network!";

    if (write(write_fd, message.c_str(), strlen(message.c_str()) + 1) < 0) {
       cout << "Network: Faile to write to system " << routers_[routers_.size() - 1].getID() << " command file descriptor." << endl;
    }

    // close(write_fd);

    return 1;
}

int Network::connectClient(DeviceInfo device_info, string router_IP) {
    for (int i = 0; i < routers_.size(); i++) {
        if (routers_[i].getIP() == router_IP) {
            client_connected_routers_[i].push_back(device_info.id_);
            this->updateLookupTable(i, device_info);
        }
    }
}

void routerProcess(Router this_router) {
    int read_fd = this_router.getCommandFd();

    int flags = fcntl(read_fd, F_GETFL, 0);
    fcntl(read_fd, F_SETFL, flags | O_NONBLOCK);

    size_t message_size = 100;
    char message[message_size];

    while (true) {
        int read_bytes = read(read_fd, message, message_size);
        if (read_bytes <= 0) {
            continue;
        }

        cout << "Router " << this_router.getID() << ": Read " << read_bytes << " bytes. The message is: " << message << endl;
        vector<string> splitted_command = splitCommand(message);
        string command_keyword = splitted_command[0];
        if (command_keyword == "connect_router") {
            string mode = splitted_command[1];
            int router_number = stoi(splitted_command[2]);
            int port_number = stoi(splitted_command[3]);

            if (mode.compare("request") == 0) {
                this_router.requestConnect(router_number, port_number);
            } else if (mode.compare("accept") == 0) {
                this_router.acceptConnect(router_number, port_number);
            }
        }
    }
}

int Network::router(vector<string> &splitted_command) {
    int fds[2];
    if (pipe(fds) < 0) {
        cout << "Network: Failed to create pipe." << endl;
    }

    int read_fd = fds[READ];
    int write_fd = fds[WRITE];

    this->routers_.push_back(Router(splitted_command[1], DEFAULT_PORTS_NUMBER, stoi(splitted_command[2]), read_fd));
    //this->routers_.push_back(Router(DEFAULT_PORTS_NUMBER, stoi(splitted_command[1]), read_fd));
    this->router_command_fd_.push_back(write_fd);
    
    int pid = fork();
    if (pid == -1) {
        cout << "ERROR: fork failed" << endl;
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        usleep(10000);
        routerProcess(routers_[routers_.size() - 1]);
        exit(EXIT_FAILURE);
    }

    string message = "Hello from Network!";

    if (write(write_fd, message.c_str(), strlen(message.c_str()) + 1) < 0) {
       cout << "Network: Faile to write to system " << routers_[routers_.size() - 1].getID() << " command file descriptor." << endl;
    }

    vector<int> connected_router;
    connected_routers_.push_back(connected_router);

    vector<int> client_connected;
    client_connected_routers_.push_back(client_connected);

    // close(write_fd);

    return 0;
}

int Network::findRouter(int router_id) {
    for (int i = 0 ; i < this->routers_.size(); i++) {
        Router router = routers_[i];
        if (router.getID() == router_id) return i;
    }
    return -1;
}

int Network::findClient(int client_id) {
    for (int i = 0; i < clients_.size(); i++) {
        if (client_id == clients_[i].getID())
            return i;
    }
    return -1;
}

int Network::createNamePipe(std::string link) {
    struct stat stats;
    if (stat(link.c_str(), &stats) < 0) {
        if (errno != ENOENT) {
            std::cout << "Network " << ": Stat failed. Error: " << errno << std::endl;
            return 1;
        }
    } else {
        if (unlink(link.c_str()) < 0) {
            std::cout << "Network " << ": Unlink failed." << std::endl;
            return 1;
        }
    }

    if (mkfifo(link.c_str(), 0666) < 0) {
        std::cout << "Network " << ": Failed to create link." << std::endl;
        return 1;
    }

    return 0;
}

int Network::connectRouter(vector<string> &splitted_command) {
    int fst_router_id = stoi(splitted_command[1]);
    int sec_router_id = stoi(splitted_command[2]);
    int fst_port_number = stoi(splitted_command[3]);
    int sec_port_number = stoi(splitted_command[4]);

    string link = "link_r_r_" + to_string(min(fst_router_id, sec_router_id)) + "_" + to_string(max(fst_router_id, sec_router_id));

    if (createNamePipe("w_"+link) || createNamePipe("r_"+link)) {
        return 1;
    }

    int fst_router_index = findRouter(fst_router_id);
    int sec_router_index = findRouter(sec_router_id);

    if (fst_router_index == -1) {
        cout << "Network: Router " << fst_router_id << " is not available." << endl;
        return 1;
    } else if (sec_router_index == -1) {
        cout << "Network: Router " << sec_router_id << " is not available." << endl;
        return 1;
    }

    string fst_router_message = "connect_router request " + to_string(sec_router_id) + " " + to_string(fst_port_number);
    int fst_router_write_fd = this->router_command_fd_[fst_router_index];

    if (write(fst_router_write_fd, fst_router_message.c_str(), strlen(fst_router_message.c_str()) + 1) < 0) {
       cout << "Network: Failed to write to router " << fst_router_id << " command file descriptor." << endl;
    }

    string sec_router_message = "connect_router accept " + to_string(fst_router_id) + " " + to_string(sec_port_number);
    int sec_router_write_fd = this->router_command_fd_[sec_router_index];

    if (write(sec_router_write_fd, sec_router_message.c_str(), strlen(sec_router_message.c_str()) + 1) < 0) {
       cout << "Network: Failed to write to router " << sec_router_id << " command file descriptor." << endl;
    }

    sleep(1);

    DeviceInfo fst_device_info;
    fst_device_info.id_ = fst_router_id;
    fst_device_info.port_number_ = fst_port_number;
    fst_device_info.IP_address_ = routers_[fst_router_index].getIP();

    DeviceInfo sec_device_info;
    sec_device_info.id_ = sec_router_id;
    sec_device_info.port_number_ = sec_port_number;
    sec_device_info.IP_address_ = routers_[sec_router_index].getIP();
    
    connected_routers_[fst_router_index].push_back(sec_router_index);
    connected_routers_[sec_router_index].push_back(fst_router_index);

    this->updateLookupTable(fst_router_index, sec_device_info);
    this->updateLookupTable(sec_router_index, fst_device_info);

    routers_[sec_router_index].updateLookupTable(routers_[fst_router_index].getLookupTable(), routers_[fst_router_index].getIP());
    routers_[fst_router_index].updateLookupTable(routers_[sec_router_index].getLookupTable(), routers_[sec_router_index].getIP());

    // TODO: add link to network

    return 0;
}

int Network::updateLookupTable(int router_index, DeviceInfo device_info) {
    routers_[router_index].updateLookupTable(device_info);
    for (int i = 0; i < connected_routers_[router_index].size(); i++) {
        if (routers_[connected_routers_[router_index][i]].getID() == device_info.id_ && device_info.type == ROUTER)
            continue;

        if (!routers_[connected_routers_[router_index][i]].hasInLookupTable(device_info)){
            device_info.IP_address_ = routers_[router_index].getIP();
            this->updateLookupTable(this->findRouter(connected_routers_[router_index][i]), device_info);
        }
    }
}

void Network::printLookupTable() {
    for (int i = 0; i < routers_.size(); i++) {
        routers_[i].printLookupTable();
    }
}