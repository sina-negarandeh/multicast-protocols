#ifndef _PACKET_HPP
#define _PACKET_HPP

#include <string>
#include <iostream>
#include <vector>

/*
packet is a 256 character string
            256
---------------------------
-   32      192      32    -
---------------------------
- Header - Data - Address -
---------------------------
*/

#define HEADER_SIZE 32
#define DATA_SIZE 192
#define ADDRESS_SIZE 32

class Packet{
    public:
    Packet(std::string packet_string);
    Packet(std::string sender_address, std::string reciever_address, std::string message);
    std::string getSenderAddress(){return sender_address_;}
    std::string getRecieverAddress(){return reciever_address_;}
    std::string getMessage(){return message_;}
    std::vector<std::string> getPacketString(){return packet_strings_;}

    private:
    std::string sender_address_;
    std::string reciever_address_;
    std::string message_;
    void makePacketString();
    void extractDataFromPacket();
    std::vector<std::string> packet_strings_;
};


#endif