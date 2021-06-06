#ifndef _PACKET_HPP
#define _PACKET_HPP

#include <string>
#include <iostream>
#include <vector>

/*
packet is a 128 character string
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
    Packet(int sender_system_id, int reciever_system_id, std::string message);
    int getSenderId(){return sender_id_;}
    int getRecieverId_(){return reciever_id_;}
    std::string getMessage(){return message_;}
    std::vector<std::string> getPacketString(){return packet_strings_;}

    private:
    int sender_id_;
    int reciever_id_;
    std::string message_;
    void makePacketString();
    void extractDataFromPacket();
    std::vector<std::string> packet_strings_;
};


#endif