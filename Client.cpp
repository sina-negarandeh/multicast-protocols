#include "Client.hpp"
#include "utils.hpp"
#include "Packet.hpp"

using namespace std;

Client::Client(std::string name, int id, IP server_IP, IP router_IP, int router_port, int command_fd): 
                                    server_IP_(server_IP), router_IP_(router_IP), self_IP_(0,0,0,0){
    name_ = name;
    router_port_ = router_port;
    command_fd_ = command_fd;
    id_ = id;
}

void Client::setIP(IP ip){
    self_IP_ = ip;
}

int Client::getID() {
    return this->id_;
}

std::string Client::getIP() {
    return this->self_IP_.get_string();
}

int Client::writeMessage(string link, string message, int router_id) {
    int fd = open(link.c_str(), O_WRONLY);

    int write_bytes = write(fd, message.c_str(), strlen(message.c_str()) + 1);
    if (write_bytes < 1) {
        cout << "Client " << id_ << ": Couldn't write a message to Router " << router_id << ": " << message << endl;
    } else {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

int Client::readMessage(string link, char* message, size_t message_size, int router_id) {
    int fd = open(link.c_str(), O_RDONLY);

    int read_bytes = read(fd, message, message_size);
    if (read_bytes > 1) {
        //string message_ =  Packet(message).getMessage();
        cout << "Client " << id_ << ": Message from Router " << router_id << ": " << message << endl;
        memset(message, 0, message_size);
        close(fd);
        return 1;
    } else {
        cout << "Client " << id_ << ": Dind't recieve message from Router " << router_id << "." << endl;
        close(fd);
        return 0;
    }
}

int Client::requestConnect(int router_id, int port_number) {
    string link = "link_c_r_" + to_string(min(id_, router_id)) + "_" + to_string(max(id_, router_id));

    string link_r = "r_" + link;
    string link_w = "w_" + link;
    
    string send_message = "Hello from Client " + to_string(id_) + ".";
    
    size_t message_size = 257;
    char message[message_size];

    int read_flag, write_flag = 0;

    while (!(read_flag && write_flag)) {
        cout << "Client " << id_ << ": Trying to open link to write." << endl;
        write_flag = this->writeMessage(link_w, send_message, router_id);
        cout << "Client " << id_ << ": Trying to open link to read." << endl;
        read_flag = this->readMessage(link_r, message, message_size, router_id);
    }
    
    cout << "Client " << id_ << ": Connect Complete" << endl;
    // TODO: add to connected list

    return 0;
}

void Client::sendFile(std::string file_name, std::string server_name){
    string message = readFileIntoString(file_name);
    //TODO: turn message into packet! maybe differant header for files!
    vector<string> packet_strings = Packet(self_IP_.get_string(),  message, server_name).getPacketString();
    //TODO: this is wrong! change this!
    string link_string = "link_" + server_name + "_" + name_;
    send(packet_strings[0], link_string);
}

void Client::sendMessage(std::string message, std::string server_name){
    //TODO: turn message into packet!
    vector<string> packet_strings = Packet(self_IP_.get_string(),  message, server_name).getPacketString(); //??????
    //TODO: this is wrong! change this!
    string link_string = "link_" + server_name + "_" + name_;
    send(packet_strings[0], link_string);
}

int Client::send(std::string message, std::string link){
    int fd = open(link.c_str(), O_WRONLY);

    int write_bytes = write(fd, message.c_str(), strlen(message.c_str()) + 1);
    if (write_bytes < 1) {
        cout << "Client " << name_ << ": Couldn't write a message to Destination!" << endl;
    } else {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}
