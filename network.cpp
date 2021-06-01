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
        if (isCommandCorrect("Router", 2, command_keyword, splitted_command.size())) {
            this->router(splitted_command);
        } else if (isCommandCorrect("RouterConnect", 5, command_keyword, splitted_command.size())) {
            this->connectRouter(splitted_command);
        } else if (isCommandCorrect("ChangeLinkCost", 3, command_keyword, splitted_command.size())) {

        } else if (isCommandCorrect("DisconnectLink", 2, command_keyword, splitted_command.size())) {

        } else if (isCommandCorrect("RouterShow", 2, command_keyword, splitted_command.size())) {

        } else {
            return 1;
        }
    } catch (exception &error) {
        cout << "Exception: " << error.what() << endl;
    }

    return 0;
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
            int port_number = stoi(splitted_command[2]);

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

    this->routers_.push_back(Router(DEFAULT_PORTS_NUMBER, stoi(splitted_command[1]), read_fd));
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

    // TODO: add link to network

    return 0;
}
