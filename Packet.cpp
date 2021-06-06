#include "Packet.hpp"
#include <string>

using namespace std;

Packet::Packet(std::string packet_string){
    packet_strings_.push_back(packet_string);
    extractDataFromPacket();
}

Packet::Packet(std::string sender_address, std::string reciever_address, std::string message){
    sender_address_ = sender_address;
    reciever_address_ = reciever_address;
    message_ = message;
    makePacketString();
}

void Packet::makePacketString(){
    string header = sender_address_;
    string address = reciever_address_;
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
    sender_address_ = sender_string.substr(0, index);
    string message_string = packet_strings_[0].substr(HEADER_SIZE, DATA_SIZE);
    index = message_string.find('#');
    message_ = message_string.substr(0, index);
    string reciever_string = packet_strings_[0].substr(HEADER_SIZE + DATA_SIZE, ADDRESS_SIZE);
    index = reciever_string.find('#');
    reciever_address_ = reciever_string.substr(0, index);
}