#include "Client.hpp"
#include "utils.hpp"
#include "Packet.hpp"

using namespace std;

Client::Client(std::string name, IP server_IP, IP router_IP, int router_port, int command_fd): 
                                    server_IP_(server_IP), router_IP_(router_IP), self_IP_(0,0,0,0){
    name_ = name;
    router_port_ = router_port;
    command_fd_ = command_fd;
}

void Client::setIP(IP ip){
    self_IP_ = ip;
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