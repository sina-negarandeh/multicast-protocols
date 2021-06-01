#include "router.hpp"

using namespace std;

Router::Router(int number_of_ports, int id, int command_fd) {
    this->number_of_ports_ = number_of_ports;
    this->id_ = id;
    this->command_fd_ = command_fd;
}

int Router::getID() {
    return this->id_;
}

int Router::getCommandFd() {
    return this->command_fd_;
}

int Router::shutdown() {
    
}

int Router::writeMessage(string link, string message, int router_id) {
    int fd = open(link.c_str(), O_WRONLY);
  
    int write_bytes = write(fd, message.c_str(), strlen(message.c_str()) + 1);
    if (write_bytes < 1) {
        cout << "Router " << id_ << ": Couldn't write a message to Router " << router_id << ": " << message << endl;
    } else {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int Router::readMessage(string link, char* message, size_t message_size, int router_id) {
    int fd = open(link.c_str(), O_RDONLY);

    int read_bytes = read(fd, message, message_size);
    if (read_bytes > 1) {
        cout << "Router " << id_ << ": Message from Router " << router_id << ": " << message << endl;
        memset(message, 0, message_size);
        close(fd);
        return 1;
    } else {
        cout << "Router " << id_ << ": Dind't recieve message from Router " << router_id << "." << endl;
        close(fd);
        return 0;
    }
}

int Router::requestConnect(int router_id, int port_number) {
    string link = "link_r_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));

    string link_r = "r_" + link;
    string link_w = "w_" + link;
    
    string send_message = "Hello from Router " + to_string(id_) + ".";
    
    size_t message_size = 128;
    char message[message_size];

    int read_flag, write_flag = 0;

    while (!(read_flag && write_flag)) {
        cout << "Router " << id_ << ": Trying to open link to write." << endl;
        write_flag = this->writeMessage(link_w, send_message, router_id);
        cout << "Router " << id_ << ": Trying to open link to read." << endl;
        read_flag = this->readMessage(link_r, message, message_size, router_id);
    }
    
    cout << "Router " << id_ << ": Connect Complete" << endl;
    // TODO: add to connected list

    return 0;
}

int Router::acceptConnect(int router_id, int port_number) {
    string link = "link_r_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));

    std::string link_r = "r_" + link;
    std::string link_w = "w_" + link;
    
    std::string send_message = "Hello from Router " + to_string(id_) + ".";
    
    size_t message_size = 128;
    char message[message_size];

    int read_flag, write_flag = 0;

    while (!(read_flag && write_flag)) {
        cout << "Router " << id_ << ": Trying to open link to read." << endl;
        read_flag = this->readMessage(link_w, message, message_size, router_id);
        cout << "Router " << id_ << ": Trying to open link to write." << endl;
        write_flag = this->writeMessage(link_r, send_message, router_id);
    }
    
    cout << "Router " << id_ << ": Connect Complete" << endl;
    // TODO: add to connected list

    return 1;
}