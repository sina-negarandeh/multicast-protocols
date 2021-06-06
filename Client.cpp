#include "Client.hpp"
#include "utils.hpp"

using namespace std;

Client::Client(std::string name, IP server_IP, IP router_IP, int router_port): server_IP_(server_IP), router_IP_(router_IP), self_IP_(0,0,0,0){
    name_ = name;
    router_port_ = router_port;
}

void Client::setIp(IP ip){
    self_IP_ = ip;
}

//TODO: we need a packet class!
void Client::sendFile(std::string file_name, std::string server_name){
    string message = readFileIntoString(file_name);
    //TODO: turn message into packet! maybe differant header for files!
    string packet_string = message;
    //TODO: this is wrong!
    string link_string = "link_" + server_name + "_" + name_;
    send(message, link_string);
}

//TODO: we need a packet class!
void Client::sendMessage(std::string message, std::string server_name){
    //TODO: turn message into packet!
    string packet_string = message;
    //TODO: this is wrong!
    string link_string = "link_" + server_name + "_" + name_;
    send(message, link_string);
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


// char data_buffer[MESSAGE_BUFFER_SIZE] = {0};
// int sendData(string file_name) {
//     FILE* file_ptr = fopen(file_name.c_str(), "r");
//     if (file_ptr == NULL) {
//         ExitWithError("File open failed!\n");
//     }
// 	int size = 0;
//     while (1) {
//         if (sendFile(file_ptr) == 1) {
//             send(client_data_socket_fd, data_buffer, strlen(data_buffer), 0);
// 			size += strlen(data_buffer);
//             break;
//         } else {
//             // send
//             send(client_data_socket_fd, data_buffer, strlen(data_buffer), 0);
// 			size += strlen(data_buffer);
//             bzero(data_buffer, MESSAGE_BUFFER_SIZE);
//         }
//     }

//     fclose(file_ptr);
// 	return size;
// }