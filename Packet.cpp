#include "Packet.hpp"
#include <string>

using namespace std;

Packet::Packet(std::string packet_string){
    packet_strings_.push_back(packet_string);
    extractDataFromPacket();
}

Packet::Packet(int sender_system_id, int reciever_system_id, std::string message){
    sender_id_ = sender_system_id;
    reciever_id_ = reciever_system_id;
    message_ = message;
    makePacketString();
}

void Packet::makePacketString(){
    string header = to_string(sender_id_);
    string address = to_string(reciever_id_);
    header.resize(HEADER_SIZE, '#');
    address.resize(ADDRESS_SIZE, '#');

    string message = message_;
    int curr_index = 0;
    while (curr_index + DATA_SIZE < message.size()){
        string sub_message = message.substr(curr_index, DATA_SIZE);
        sub_message.resize(DATA_SIZE, '#');
        string packet_string = header + sub_message + address;
        packet_strings_.push_back(packet_string);
        curr_index += DATA_SIZE;
    }
    string sub_message = message.substr(curr_index);
    sub_message.resize(DATA_SIZE, '#');
    string packet_string = header + sub_message + address;
    packet_strings_.push_back(packet_string);
}

void Packet::extractDataFromPacket(){
    string sender_string = packet_strings_[0].substr(0, HEADER_SIZE);
    int index = sender_string.find('#');
    sender_id_ = stoi(sender_string.substr(0, index));
    string message_string = packet_strings_[0].substr(HEADER_SIZE, DATA_SIZE);
    index = message_string.find('#');
    message_ = message_string.substr(0, index);
    string reciever_string = packet_strings_[0].substr(HEADER_SIZE + DATA_SIZE, ADDRESS_SIZE);
    index = reciever_string.find('#');
    reciever_id_ = stoi(reciever_string.substr(0, index));
}