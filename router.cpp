#include "router.hpp"

using namespace std;

Router::Router(string router_IP_address, int number_of_ports, int id, int command_fd) {
//Router::Router(int number_of_ports, int id, int command_fd) {
    this->number_of_ports_ = number_of_ports;
    this->id_ = id;
    this->command_fd_ = command_fd;
    this->router_IP_address_ = router_IP_address;
}

int Router::getID() {
    return this->id_;
}

int Router::getCommandFd() {
    return this->command_fd_;
}

string Router::getIP() {
    return this->router_IP_address_;
}

int Router::shutdown() {
    //TODO: complete
    return 0;
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
        //string message_ =  Packet(message).getMessage();
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
    
    size_t message_size = 257;
    char message[message_size];

    int read_flag, write_flag = 0;

    while (!(read_flag && write_flag)) {
        cout << "Router " << id_ << ": Trying to open link to write." << endl;
        write_flag = this->writeMessage(link_w, send_message, router_id);
        cout << "Router " << id_ << ": Trying to open link to read." << endl;
        read_flag = this->readMessage(link_r, message, message_size, router_id);
    }
    
    cout << "Router " << id_ << ": Connect Complete" << endl;
    DeviceInfo router;
    router.id_ = router_id;
    router.port_number_ = port_number;
    connected_routers_.push_back(router);

    // TODO: add to connected list

    return 0;
}

int Router::acceptConnect(int router_id, int port_number, int device_type) {
    string link = "";
    if (device_type == ROUTER)
        link = "link_r_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));
    else if (device_type == SYSTEM) {
        link = "link_c_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));
        link_ = to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));
    }

    std::string link_r = "r_" + link;
    std::string link_w = "w_" + link;
    
    std::string send_message = "Hello from Router " + to_string(id_) + ".";
    
    size_t message_size = 257;
    char message[message_size];

    int read_flag, write_flag = 0;

    while (!(read_flag && write_flag)) {
        cout << "Router " << id_ << ": Trying to open link to read." << endl;
        read_flag = this->readMessage(link_w, message, message_size, router_id);
        cout << "Router " << id_ << ": Trying to open link to write." << endl;
        write_flag = this->writeMessage(link_r, send_message, router_id);
    }
    
    cout << "Router " << id_ << ": Connect Complete" << endl;
    if (device_type == ROUTER) {
        DeviceInfo router;
        router.id_ = router_id;
        router.port_number_ = port_number;
        connected_routers_.push_back(router);
    }
    // TODO: add to connected list

    return 1;
}

bool Router::hasInLookupTable(DeviceInfo device_info) {
    for (int i = 0; i < this->lookup_table_.size(); i++) {
        if (lookup_table_[i].id_ == device_info.id_ && lookup_table_[i].type == device_info.type)
            return true;
    }
    
    return false;
}

int Router::updateLookupTable(DeviceInfo device_info) {
    if (this->hasInLookupTable(device_info)) {
        return 0;
    }
    this->lookup_table_.push_back(device_info);
}

void Router::printLookupTable() {
    cout << "Router " << this->id_ << " Lookup Table" << endl;
    for (int i = 0; i < this->lookup_table_.size(); i++) {
        cout << "---------------------" << endl;
        if (lookup_table_[i].type == ROUTER)
            cout << "Router " << lookup_table_[i].id_ << " IP: " << lookup_table_[i].IP_address_ << endl;
        else if (lookup_table_[i].type == SYSTEM)
            cout << "Client IP: " << lookup_table_[i].IP_address_ << endl;
        else if (lookup_table_[i].type == SERVER)
            cout << "Server IP: " << lookup_table_[i].IP_address_ << endl;
    }
    cout << "---------------------\n" << endl;
}

std::vector<DeviceInfo> Router::getLookupTable() {
    return lookup_table_;
}

int Router::updateLookupTable(std::vector<DeviceInfo> lookup_table, string IP) {
    for (int i = 0; i < lookup_table.size(); i++) {
        lookup_table[i].IP_address_ = IP;
        this->updateLookupTable(lookup_table[i]);
    }
    return 0;
}

void Router::receive() {
    fd_set readfds;
    FD_ZERO(&readfds);

    std::string link_w = "w_" + link_;
    
    // cout << "Switch " << switch_number_ << ": " << link << endl;
    
    size_t message_size = 129;
    char message[message_size];

    // cout << "Switch " << switch_number_ << ": Trying to open link to read." << endl;
    int fd = open(link_w.c_str(), O_RDONLY|O_NONBLOCK);

    FD_SET(fd, &readfds);

    if (FD_ISSET(fd, &readfds)) {
        
        int read_bytes = read(fd, message, message_size);
        if (read_bytes > 0) {
            cout << "Router " << id_ << ": Message from Client " << ": " << message << endl;
            

            //TODO: Forward here
            //this->send();

            memset(message, 0, message_size);
            close(fd);
        }

        
    } else {
        close(fd);
    }

    for (int router_index = 0; router_index < this->connected_routers_.size(); router_index++) {
        int router_id = this->connected_routers_[router_index].id_;

        string link = "link_r_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));

        std::string link_w = "w_" + link;
        
        size_t message_size = 129;
        char message[message_size];

        int fd = open(link_w.c_str(), O_RDONLY|O_NONBLOCK);

        FD_SET(fd, &readfds);

        if (FD_ISSET(fd, &readfds)) {
            
            int read_bytes = read(fd, message, message_size);
            if (read_bytes > 0) {
                cout << "Router " << id_ << ": Message from System " << router_id << ": " << message << endl;

                memset(message, 0, message_size);
                close(fd);
            }
        } else {
            close(fd);
        }
    }
}
